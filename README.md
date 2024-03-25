# 간단한 어셈블러 만들기

SIC는 8비트가 한 바이트로 이루어져 있다. 모든 주소는 바이트로 표현되며 저장된다.  
3개의 이어진 Bytes를 Word라고 하며, Word를 구성하는 3바이트 중 최하위 바이트의 위치로 주소가 지정된다.  
SIC의 전체 메모리는 2의 15승이다.  
자세한 내용은 다음 블로그를 참조하자. => <https://dev.youngkyu.kr/tag/RESW>

---

교육용으로 설계된 다음과 같은 어셈블리 언어를 컴파일하고  
목적코드를 생성할 수 있는 컴파일러가 설계되고 구현되어야 한다.<br><br>

COPY START 2000  
  ** LDA FIVE  
  ** STA ALPHA  
  ** LDCH CHARZ  
  ** STCH C1  
  ALPHA RESW 1  
  FIVE WORD 5  
  CHARZ BYTE C'EOF'  
  C1 RESB 1  
  ** END **<br><br>

  * 'COPY START 2000' : 코드의 시작을 의미하고 2000번지부터 시작함을 나타낸다.
  * '** END **' : 코드의 마지막을 의미한다.
  * 코드의 처음이 '**'로 시작하면 명령어이다.
  * 명령어는 LDA, STA, LDCH, STCH 4개만 존재한다.
    - LDA FIVE : A register <- FIVE의 값
    - STA ALPHA : ALPHA <- A register의 값
    - LDCH CHARZ : A register의 값 (rightmost byte) <- CHARZ의 값
    - STCH C1 : C1 (byte) <- A register의 값 (rightmost byte)
  * Storage는 다음과 같이 정의한다.
    - BYTE : 문자 혹은 16진수 상수를 생성한다.
    - WORD : 한 워드 정수 상수를 생성한다.
    - RESB : 지정해준 수만큼의 바이트를 예약한다.
    - RESW : 지정해준 수만큼의 워드들을 예약한다.

      예를 들어보자
      + ALPHA RESQ 1 : 1 만큼의 워드를 예약
      + FIVE WORD 5 : 상수 5 생성
      + CHARZ BYTE C'EOF' : 문자상수 EOF 생성
      + C1 RESB 1 : 1 만큼의 바이트 예약

  ---

  위의 코드는 입력으로 받아 컴파일할 어셈블리 코드이다.  
  위 코드를 컴파일하여 다음과 같은 결과를 출력해야 한다.<br><br>

  The contents of Input file:

  COPY START 2000  
  ** LDA FIVE  
  ** STA ALPHA  
  ** LDCH CHARZ  
  ** STCH C1  
  ALPHA RESW 1  
  FIVE WORD 5  
  CHARZ BYTE C'EOF'  
  C1 RESB 1  
  ** END **<br><br>
  
  Length of the input program is 20.<br>
  
  The contents of Symbol Table:

  ALPHA 2012  
  FIVE 2015  
  CHARZ 2018  
  C1 2019<br><br>

  The contents of Intermed File:

  COPY START 2000  
  2000 ** LDA FIVE  
  2003 ** STA ALPHA  
  2006 ** LDCH CHARZ  
  2009 ** STCH C1  
  2012 ALPHA RESW 1  
  2015 FIVE WORD 5  
  2018 CHARZ BYTE C'EOF'  
  2019 C1 RESB 1  
  2020 ** END **

  Object Program has been generated.

  Object Program:

  H^COPY^002000^002020  
  T^002000^19^332015^442012^532018^572019^000005^454f46  
  E^002000
