# DBProject

## 프로젝트 목적
이 프로젝트는 관계형 데이터베이스와 관련 응용 프로그램의 전체 생애 주기 - 개념 설계, 논리 설계, 구현, 그리고 운영 및 유지 보수 - 에 대한 실질적인 경험을 제공하는 것을 목표로 합니다.

## 요구사항
본 프로젝트는 온라인 쇼핑몰과 오프라인 매장을 운영하는 상업적 환경의 데이터베이스 시스템을 설계하고 구현합니다. 시스템은 다음과 같은 기능을 포함해야 합니다:

### 1. 구조
시스템은 온라인 상점, 오프라인 지점, 그리고 중앙 창고로 구성되어 있습니다.

### 2. 상품 분류
다양한 상품들은 제품군, 제조사 등 여러 기준에 따라 분류되어야 합니다.

### 3. 재고 관리
창고와 각 지점에서는 상품의 재고 상태를 파악하며, 재고가 부족한 상품에 대해서는 제조업체로부터 주문을 처리합니다. 물품 도착 시 재고 정보가 업데이트되어야 합니다.

### 4. 배송 관리
온라인에서 구매된 제품은 택배사를 통해 전달되며, 배송 번호는 저장되어 고객이 주문 상태를 조회할 수 있어야 합니다.

### 5. 매출 분석
기간, 제품, 제품 그룹, 계절, 매장 등을 기준으로 매출 데이터를 분석할 수 있도록 매출 정보를 저장합니다.

### 6. 결제 관리
고객은 각각의 구매 시점마다 결제를 할 수도 있고, 월말에 한 달 동안의 구매 합계를 결제할 수도 있습니다. 이에 따라, 특정 고객이 월말에 얼마를 지불해야 하는지에 대한 정보를 제공합니다.

### 7. 카드 정보 저장
온라인 고객의 경우 카드 정보를 안전하게 저장하고 있어야 합니다.

## 구현해야 하는 쿼리 목록

1. **배송 사고 발생 시, 해당 배송에 연관된 고객 정보 조회**
2. **배송 사고 발생 시, 해당 고객의 주문에 대해 새 주문 생성**
3. **지난 해 가장 많이 구매한 고객 정보 조회**
4. **지난 해 가장 많이 판매된 제품 정보 조회**
5. **지난 해 판매된 모든 제품 정보 조회**
6. **가장 높은 매출을 기록한 상위 k개 품목 조회**
7. **전체 품목 중 매출이 상위 10%에 속하는 품목 조회**
8. **가장 많이 판매된 수량을 가진 상위 k개 품목 조회**
9. **전체 품목 중 판매 수량이 상위 10%에 속하는 품목 조회**
10. **모든 지점에서 품절된 품목 정보 조회**
11. **예정된 배송 시간을 초과하여 배송되지 않은 주문 정보 조회**
12. **월말에 청구가 예정된 고객들의 결제 영수증 생성**