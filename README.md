# Project Vertical

> **기간 :** 2025.02.06 ~ 2025.03.05
> 

> **팀 규모 :** 2인
> 

> **기술 스택 :** Unreal Engine 5, C++
> 

> **역할 :** 플레이어 시스템 개발
> 

> **장르 :** 3인칭 오픈월드 액션 RPG (Horizon 시리즈 모작)
> 

> 
> 

[플레이어 구현 기능 요약](/Resources/summary%201.csv)

## 주요 구현 상세

### **1. 카메라 전환 시스템**

- TPS 시점에서 `TransitionCamera`를 활용해 전투 / 탐색 시야 전환
- 부드러운 전환을 위해 `ViewCamera1`, `ViewCamera2` 설정 및 보간 처리

### **2. 활 메커니즘**

![image.png](/Resources/image.png)

- 활 시위는 손가락 위치 기준으로 Bowstring 위치를 계산
- 화살은 활에서 `Attach` 후 발사 시 `Detach` → `Velocity` 적용

### **3. 트립 캐스터 (특수 무기)**

![image.png](/Resources/image%201.png)

- 두 개의 화살을 미리 생성
- 하나는 케이블 연결, 하나는 `Hidden`
- 발사 후 충돌지점에 다른 화살이 `Cable`로 연결되어 트랩 완성

### **4. 피격 및 죽음 시스템**

![image.png](/Resources/image%202.png)

- 공격 방향과 플레이어 방향 벡터의 내적을 계산해 타격 반응 방향 판별
- 사망 처리 시 전용 애니메이션 및 상태 전환 처리

### **5. 근접 공격**

![image.png](/Resources/image%203.png)

- `AnimNotifyState` 기반 콤보 모션 연출
- 입력 타이밍에 따라 다음 공격으로 자연스럽게 연결

### 6. 구르기 시스템

![스크린샷 2025-02-20 112946.png](/Resources/Image%204.png)

- `Enhanced Input`의 `Combo Key Trigger`를 활용해 빠르게 두 번 입력 시 회피 동작을 트리거
- `Combo Actions` 배열 내에 `IA_PlayerDodge` 액션을 2회 연속 등록
- 각 스텝은 `Combo Step Action`, `Completion States`, `Time to Press Key` 등을 통해 세밀히 제어

## 사용 기술 및 툴

- Unreal Engine 5 (C++, Blueprint 혼합)
- Animation Montage / Notify
- Niagara Effect (공통 시스템 연출)
- UE5 Physics 및 Cable Component 활용
- Overlay Material (Focus Mode 구현)

## 회고 및 어려움

- 트립 캐스터 구현 시 어려웠던 점: 케이블 피직스 충돌과 Arrow 위치 연산의 정밀도 문제
- 피격 방향 처리: 단순 애니메이션이 아닌 물리 기반 방향 판별로 몰입감 있는 리액션 구현
