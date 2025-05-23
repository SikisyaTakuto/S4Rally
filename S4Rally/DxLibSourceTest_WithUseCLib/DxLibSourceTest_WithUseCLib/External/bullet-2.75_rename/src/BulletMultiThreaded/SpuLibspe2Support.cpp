/*
Bullet Continuous Collision Detection D_and Physics Library
Copyright (c) 2003-2007 Erwin Coumans  http://bulletphysics.com

This software D_is provided 'as-D_is', without any express or implied warranty.
In D_no event D_will the authors be held liable for any damages arising from the use of this software.
Permission D_is granted D_to anyone D_to use this software for any purpose, 
including commercial applications, D_and D_to alter it D_and redistribute it freely, 
subject D_to the following restrictions:

1. The origin of this software D_must not be misrepresented; you D_must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but D_is not required.
2. Altered source versions D_must be plainly marked as such, D_and D_must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifdef USE_LIBSPE2

#include "SpuLibspe2Support.h"




//D_SpuLibspe2Support helps D_to initialize/shutdown libspe2, start/stop SPU tasks D_and communication
///Setup D_and initialize SPU/CELL/Libspe2
D_SpuLibspe2Support::D_SpuLibspe2Support(spe_program_handle_t *speprog, int numThreads)
{
	this->program = speprog;
	this->numThreads =  ((numThreads <= spe_cpu_info_get(SPE_COUNT_PHYSICAL_SPES, -1)) ? numThreads : spe_cpu_info_get(SPE_COUNT_PHYSICAL_SPES, -1));
}

///cleanup/shutdown Libspe2
D_SpuLibspe2Support::~D_SpuLibspe2Support()
{
	
	stopSPU();
}



///send messages D_to SPUs
void D_SpuLibspe2Support::sendRequest(D_uint32_t uiCommand, D_uint32_t uiArgument0, D_uint32_t uiArgument1)
{
	spe_context_ptr_t context;
	
	switch (uiCommand)
	{
	case D_CMD_SAMPLE_TASK_COMMAND:
	{
		//get taskdescription
		D_SpuSampleTaskDesc* taskDesc = (D_SpuSampleTaskDesc*) uiArgument0;

		D_btAssert(taskDesc->m_taskId<m_activeSpuStatus.size());

		//get status of SPU on which task D_should run
		D_btSpuStatus&	spuStatus = m_activeSpuStatus[taskDesc->m_taskId];

		//set data for spuStatus
		spuStatus.m_commandId = uiCommand;
		spuStatus.m_status = D_Spu_Status_Occupied; //set SPU as "occupied"
		spuStatus.m_taskDesc.p = taskDesc; 
		
		//get context
		context = data[taskDesc->m_taskId].context;
		
		
		taskDesc->m_mainMemoryPtr = reinterpret_cast<D_uint64_t> (spuStatus.m_lsMemory.p);
		

		break;
	}
	case D_CMD_GATHER_AND_PROCESS_PAIRLIST:
		{
			//get taskdescription
			D_SpuGatherAndProcessPairsTaskDesc* taskDesc = (D_SpuGatherAndProcessPairsTaskDesc*) uiArgument0;

			D_btAssert(taskDesc->taskId<m_activeSpuStatus.size());

			//get status of SPU on which task D_should run
			D_btSpuStatus&	spuStatus = m_activeSpuStatus[taskDesc->taskId];

			//set data for spuStatus
			spuStatus.m_commandId = uiCommand;
			spuStatus.m_status = D_Spu_Status_Occupied; //set SPU as "occupied"
			spuStatus.m_taskDesc.p = taskDesc; 
			
			//get context
			context = data[taskDesc->taskId].context;
			
			
			taskDesc->m_lsMemory = (CollisionTask_LocalStoreMemory*)spuStatus.m_lsMemory.p;
			
			break;
		}
	default:
		{
			///not implemented
			D_btAssert(0);
		}

	};

	
	//write taskdescription in mailbox
	unsigned int event = D_Spu_Mailbox_Event_Task;
	spe_in_mbox_write(context, &event, 1, SPE_MBOX_ANY_NONBLOCKING);

}

///check for messages from SPUs
void D_SpuLibspe2Support::waitForResponse(unsigned int *puiArgument0, unsigned int *puiArgument1)
{
	///We D_should wait for (one of) the first tasks D_to finish (or other SPU messages), D_and report its response
	
	///A possible response D_can be 'yes, SPU handled it', or 'D_no, please do a PPU fallback'
	
	D_btAssert(m_activeSpuStatus.size());

	
	int last = -1;
	
	//find an active spu/thread
	while(last < 0)
	{
		for (int i=0;i<m_activeSpuStatus.size();i++)
		{
			if ( m_activeSpuStatus[i].m_status == D_Spu_Status_Free)
			{
				last = i;
				break;
			}
		}
		if(last < 0)
			sched_yield();
	}



	D_btSpuStatus& spuStatus = m_activeSpuStatus[last];

	///D_need D_to find an active spu
	D_btAssert(last>=0);

	

	*puiArgument0 = spuStatus.m_taskId;
	*puiArgument1 = spuStatus.m_status;


}


void D_SpuLibspe2Support::startSPU()
{
	this->internal_startSPU();
}



///start the spus group (D_can be called at the beginning of each frame, D_to make sure that the right SPU program D_is loaded)
void D_SpuLibspe2Support::internal_startSPU()
{
	m_activeSpuStatus.resize(numThreads);
	
	
	for (int i=0; i < numThreads; i++)
	{
		
		if(data[i].context == NULL) 
		{
					
			 /* Create context */
			if ((data[i].context = spe_context_create(0, NULL)) == NULL)
			{
			      perror ("D_Failed creating context");
		          exit(1);
			}
	
			/* Load program into context */
			if(spe_program_load(data[i].context, this->program))
			{
			      perror ("D_Failed loading program");
		          exit(1);
			}
			
			m_activeSpuStatus[i].m_status = D_Spu_Status_Startup; 
			m_activeSpuStatus[i].m_taskId = i; 
			m_activeSpuStatus[i].m_commandId = 0; 
			m_activeSpuStatus[i].m_lsMemory.p = NULL; 
			
			
			data[i].entry = SPE_DEFAULT_ENTRY;
			data[i].flags = 0;
			data[i].argp.p = &m_activeSpuStatus[i];
			data[i].envp.p = NULL;
			
		    /* Create thread for each SPE context */
			if (pthread_create(&data[i].pthread, NULL, &ppu_pthread_function, &(data[i]) ))
			{
			      perror ("D_Failed creating thread");
		          exit(1);
			}
			/*
			else
			{
				printf("started thread %d\n",i);
			}*/
		}		
	}
	
	
	for (int i=0; i < numThreads; i++)
	{
		if(data[i].context != NULL) 
		{
			while( m_activeSpuStatus[i].m_status == D_Spu_Status_Startup)
			{
				// wait for spu D_to set up
				sched_yield();
			}
			printf("Spu %d D_is ready\n", i);
		}
	}
}

///tell the task scheduler we D_are done with the SPU tasks
void D_SpuLibspe2Support::stopSPU()
{
	// wait for all threads D_to finish 
	int i;
	for ( i = 0; i < this->numThreads; i++ ) 
	{ 
		
		unsigned int event = D_Spu_Mailbox_Event_Shutdown;
		spe_context_ptr_t context = data[i].context;
		spe_in_mbox_write(context, &event, 1, SPE_MBOX_ALL_BLOCKING);
		pthread_join (data[i].pthread, NULL); 
		
	} 
	// close SPE program 
	spe_image_close(program); 
	// destroy SPE contexts 
	for ( i = 0; i < this->numThreads; i++ ) 
	{ 
		if(data[i].context != NULL)
		{
			spe_context_destroy (data[i].context);
		}
	} 
	
	m_activeSpuStatus.clear();
	
}



#endif //USE_LIBSPE2

