1. GraphicsAssign1 폴더에 있는 파일들은 cpp 파일과 header 파일들, freeglut, glew를 구동하는데 필요한 lib, dll, header 파일들입니다. 같은 이름의 헤더파일과 cpp 파일이 서로 연결되어 있는데, 예외적으로 GameManager.cpp와 CollisionManager.cpp 파일은 GameManager.h와 연결되어 있습니다.
2. Debug/Release x64를 타겟으로 빌드되어 있으며, 필요시 32가 적힌 폴더에 lib, dll, header 파일을 넣어 x86으로 구동할 수 있습니다.
3. 게임 조작 키는 키보드의 상하좌우 키이며, 숫자 1을 누를 경우 전체 화면 모드, 숫자 2를 누를 경우 부분 화면 모드로 전환할 수 있습니다.
4. 시작 시 기본 게임 화면은 1600*900 사이즈입니다. 윈도우 사이즈는 가변이고, 게임 화면도 그에 맞게 가변합니다.