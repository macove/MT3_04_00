#include <Novice.h>
#include "GeometryUtility.h"
#include <imgui.h>
const char kWindowTitle[] = "学籍番号";

GeometryUtility geometryUtility;

struct  Spring
{
	Vector3 anchor;
	float naturalLength;
	float stiffness;
	float dampingCoefficient;
};

struct  Ball
{
	Vector3 position;
	Vector3 velocity;
	Vector3 acceleration;
	float mass;
	float radius;
	unsigned int color;
};

Vector3 operator+ (const Vector3& v1, const Vector3& v2) { return geometryUtility.Add(v1, v2); }
Vector3 operator- (const Vector3& v1, const Vector3& v2) { return geometryUtility.Subtract(v1, v2); }
Vector3 operator* (const Vector3& v, const float s) { return geometryUtility.Multiply(s, v); }

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Vector3 rotate{ 0.0f,0.0f,0.0f };

	Vector3 translate{ 0.0f,0.0f,0.0f };


	int kWindowWidth = 1280;
	int kWindowHeight = 720;

	float radius = 10.0f;
	float theta = -3.46f;
	float phi = 1.5f;

	Spring spring = { {0.0f, 0.0f, 0.0f}, 1.0f, 100.0f, 2.0f };
	Ball ball = { {1.2f, 0.0f, 0.0f}, {}, {}, 2.0f, 0.05f, BLUE };

	Vector3 origin = { 0.0f,0.0f,0.0f };

	float deltaTime = 1.0f / 60.0f;

	bool isSimulationRunning = false;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		

		if (isSimulationRunning) {
		Vector3 diff = ball.position - spring.anchor;
		float length = geometryUtility.length(diff);
		if (length != 0.0f) {
			Vector3 direction = geometryUtility.normalize(diff);
			Vector3 restPosition = spring.anchor + direction * spring.naturalLength;
			Vector3 displacement = geometryUtility.Multiply(length , (ball.position - restPosition));
			Vector3 restoringForce = geometryUtility.Multiply(-spring.stiffness, displacement);
			Vector3 dampingForce = geometryUtility.Multiply(-spring.dampingCoefficient ,ball.velocity);
			Vector3 force = restoringForce + dampingForce;

			ball.acceleration = geometryUtility.divide(force , ball.mass);
			ball.velocity = geometryUtility.Add(ball.velocity,(ball.acceleration * deltaTime));
			ball.position = geometryUtility.Add(ball.position,(ball.velocity * deltaTime));
		}

		}

		Vector3 cameraPosition = geometryUtility.SphericalToCartesian(radius, theta, phi);
		Vector3 cameraTarget = { 0.0f, 0.0f, 0.0f };
		Vector3 cameraUp = { 0.0f, 1.0f, 0.0f };

		Matrix4x4 viewMatrix = geometryUtility.MakeLookAtMatrix4x4(cameraPosition, cameraTarget, cameraUp);
		Matrix4x4 worldMatrix = geometryUtility.MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);
		Matrix4x4 projectionMatrix = geometryUtility.MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = geometryUtility.Multiply(worldMatrix, geometryUtility.Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewProjectionMatrix = geometryUtility.Multiply(viewMatrix, projectionMatrix);
		Matrix4x4 viewportMatrix = geometryUtility.MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);


		Vector3 originP = geometryUtility.Transform(geometryUtility.Transform(origin, viewProjectionMatrix), viewportMatrix);
		Vector3 ballPos = geometryUtility.Transform(geometryUtility.Transform(ball.position, viewProjectionMatrix), viewportMatrix);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		geometryUtility.DrawGrid(viewProjectionMatrix, viewportMatrix);

		geometryUtility.DrawSphere({ ball.position,0.1f }, viewProjectionMatrix, viewportMatrix,0x0000FFFF);
		Novice::DrawLine(int(originP.x), int(originP.y), int(ballPos.x), int(ballPos.y), 0xFFFFFFFF);

		ImGui::Begin("Window");
		ImGui::DragFloat("theta", &theta, 0.01f);
		ImGui::DragFloat("phi", &phi, 0.01f);
		if (ImGui::Button("Start")) {
			isSimulationRunning = true;
		}
		if (ImGui::Button("Stop")) {
			isSimulationRunning = false;
		}
		//ImGui::DragFloat3("translates[0]", &translates[0].x, 0.01f);
		//ImGui::DragFloat3("rotates[0]", &rotates[0].x, 0.01f);
		//ImGui::DragFloat3("scales[0]", &scales[0].x, 0.01f);
		//ImGui::DragFloat3("translates[1]", &translates[1].x, 0.01f);
		//ImGui::DragFloat3("rotates[1]", &rotates[1].x, 0.01f);
		//ImGui::DragFloat3("scales[1]", &scales[1].x, 0.01f);
		//ImGui::DragFloat3("translates[2]", &translates[2].x, 0.01f);
		//ImGui::DragFloat3("rotates[2]", &rotates[2].x, 0.01f);
		//ImGui::DragFloat3("scales[2]", &scales[2].x, 0.01f);
		ImGui::End();

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
