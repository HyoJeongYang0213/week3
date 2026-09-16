#pragma once

#include "FVertexSimple.h"
#include <vector>
#include <cmath>

// 전구(Light Bulb) 아이콘 메시 생성 함수
inline TArray<FVertexData> CreateBulbVertices()
{
    TArray<FVertexData> Vertices;

    const float pi = 3.14159265358979323846f;
    constexpr int Slices = 12;

    struct FRing
    {
        float y;
        float radius;
    };

    // 전구 단면 프로파일 (위 → 아래)
    // 유리구 지름 0.80 / 소켓 지름 0.34 → 약 43%
    const FRing Profile[] =
    {
        // --- 유리구 (둥근 몸통) ---
        { 0.52f, 0.00f },  // 꼭대기 (닫힘)
        { 0.50f, 0.11f },
        { 0.46f, 0.20f },
        { 0.40f, 0.28f },
        { 0.32f, 0.35f },
        { 0.22f, 0.39f },
        { 0.12f, 0.40f },  // 최대 폭
        { 0.02f, 0.39f },
        { -0.08f, 0.35f },
        { -0.16f, 0.29f },
        { -0.23f, 0.22f },  // 어깨에서 좁아짐

        // --- 목 ---
        { -0.28f, 0.17f },
        { -0.33f, 0.16f },

        // --- 나사 소켓 ---
        { -0.36f, 0.18f },  // 플랜지
        { -0.50f, 0.17f },  // 몸통 (거의 직선)
        { -0.56f, 0.15f },

        // --- 접점 ---
        { -0.60f, 0.10f },
        { -0.63f, 0.00f },  // 끝 (닫힘)
    };
    constexpr int RingCount = sizeof(Profile) / sizeof(Profile[0]);

    // 공통 정점 생성 (법선 정규화 포함)
    auto MakeVertex = [](float x, float y, float z,
        float nx, float ny, float nz) -> FVertexData
        {
            FVertexData V;

            V.x = z;  V.y = x;  V.z = y;
            V.r = 1.0f;  V.g = 1.0f;  V.b = 1.0f;  V.a = 1.0f;
            V.u = 0.0f;  V.v = 0.0f;

            const float Length = sqrtf(nx * nx + ny * ny + nz * nz);
            if (Length > 0.00001f)
            {
                nx /= Length;
                ny /= Length;
                nz /= Length;
            }

            V.nx = nz;  V.ny = nx;  V.nz = ny;
            return V;
        };

    // 링 두 개를 잇는 띠 생성 (회전면)
    auto AddRingBand = [&](float y0, float r0, float y1, float r1)
        {
            const float drdy = (r1 - r0) / (y1 - y0);
            const float ny = -drdy;

            for (int slice = 0; slice < Slices; ++slice)
            {
                const float theta0 = 2.0f * pi * slice / Slices;
                const float theta1 = 2.0f * pi * (slice + 1) / Slices;

                const float c0 = cosf(theta0);
                const float s0 = sinf(theta0);
                const float c1 = cosf(theta1);
                const float s1 = sinf(theta1);

                FVertexData V00 = MakeVertex(r0 * c0, y0, r0 * s0, c0, ny, s0);
                FVertexData V01 = MakeVertex(r0 * c1, y0, r0 * s1, c1, ny, s1);
                FVertexData V10 = MakeVertex(r1 * c0, y1, r1 * s0, c0, ny, s0);
                FVertexData V11 = MakeVertex(r1 * c1, y1, r1 * s1, c1, ny, s1);

                // CW 와인딩 (FrontCounterClockwise = FALSE)
                Vertices.Add(V00);
                Vertices.Add(V11);
                Vertices.Add(V10);

                Vertices.Add(V00);
                Vertices.Add(V01);
                Vertices.Add(V11);
            }
        };

    for (int ring = 0; ring < RingCount - 1; ++ring)
    {
        AddRingBand(Profile[ring].y, Profile[ring].radius,
            Profile[ring + 1].y, Profile[ring + 1].radius);
    }

    return Vertices;
}

inline const TArray<FVertexData> bulbVertices = CreateBulbVertices();