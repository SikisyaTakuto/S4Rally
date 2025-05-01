//wheelColliderのヘッダファイル
#pragma once

//ヘッダファイル読み込み
#include "DxLib.h"
#include "map.h"
#include "car.h"

class WheelCollider {
public:
    // 基本プロパティ
    float mass;                  //ホイールの質量
    float radius;                //ホイールの半径
    float wheelDampingRate;      //ホイールに適用される減衰値
    float suspensionDistance;    //ホイールサスペンションの最大延長距離
    float forceAppPointDistance; //ホイールの力が適用される位置を定義
    VECTOR center;               //オブジェクトのホイールの中心。

    /// <summary>
    /// ホイールが前転する際のタイヤの摩擦のプロパティ
    /// </summary>
    struct ForwardFriction {
        float extremumSlip;    //曲線の極値点   
        float extremumValue;   //曲線の極値点   
        float asymptoteSlip;   //摩擦曲線の漸近線のスリップ値
        float asymptoteValue;  //摩擦曲線の漸近線のフォース値
        float stiffness;       //Extremum Value と Asymptote Value に対する乗数 (デフォルトは 1)。 0に設定すると、ホイールからのすべての摩擦が完全に無効
    }forwardFriction;
    
    /// <summary>
    /// ホイールが横転する際のタイヤの摩擦のプロパティ
    /// </summary>
    struct SidewaysFriction {
        float extremumSlip;    //曲線の極値点          
        float extremumValue;   //曲線の極値点          
        float asymptoteSlip;   //摩擦曲線の漸近線のスリップ値
        float asymptoteValue;  //摩擦曲線の漸近線のフォース値
        float stiffness;       //Extremum Value と Asymptote Value に対する乗数 (デフォルトは 1)。 0に設定すると、ホイールからのすべての摩擦が完全に無効
    }sidewaysFriction;


    // コンストラクタ・デストラクタ 
    WheelCollider();   // 初期化
    ~WheelCollider();  // 終了処理（今回は特にリソース開放は不要）

    // サスペンション計算＋地面との接触
    bool GetGroundHit(const VECTOR& wheelPosition, VECTOR& hitPoint, VECTOR& hitNormal, float& suspensionCompression);

    // 摩擦力計算
    VECTOR CarCulateFrictionForce(const VECTOR& velocity, float suspensionCompression, float sideBrakeInput);

private:
    // 地面との判定(簡易版: 地面Y=0固定）
    bool Raycast(const VECTOR& origin, const VECTOR& direction, float maxDistance, VECTOR& hitPoint, VECTOR& hitNormal);

    // Clamp関数：値を[min, max]の範囲に収める
    float Clamp(float value, float min, float max);
};