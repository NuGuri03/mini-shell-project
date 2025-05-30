# 🛠️ C 기반 커맨드 프롬프트 프로젝트 커리큘럼 (C 중심, C++ 보완)

## 📦 전체 흐름 요약

| 단계 | 주 언어 | 핵심 키워드 |
|------|---------|-------------|
| 1단계 | ✅ C | 기본 쉘 구조 |
| 2단계 | ✅ C | 파이프, 리디렉션 |
| 3단계 | ✅ C | 프롬프트, 환경변수 |


## 🔹 1단계: C 기반 쉘의 기본 구조 만들기 (🟢 메인 단계)

### 🎯 목표
- 문자열 입력 → 파싱 → 실행 구조 구성
- `cd`, `pwd`, `exit` 등 내부 명령어 직접 구현
- `ls`, `date` 등 외부 명령 실행 (`fork` + `execvp`)

### 🧩 기술 키워드
- `fgets`, `strtok`, `strcmp`
- `fork`, `execvp`, `wait`
- `chdir`, `getcwd`

### 🛠 과제
- `main.c`, `parser.c`, `executor.c` 모듈 구성
- 테스트 입력 자동화 (`< test.txt` 활용)
- Makefile로 전체 빌드


## 🔹 2단계: 기능 확장 - 입출력 리디렉션, 파이프, 백그라운드 (🟢 C로 계속)

### 🎯 목표
- 쉘다운 기능 구현: `|`, `<`, `>`, `&` 등의 연산자 지원

### 🧩 기술 키워드
- `pipe()`, `dup2()`, `open()`, `O_CREAT`
- `&` 처리: `fork()` 후 `wait()` 생략

### 🛠 과제
- `ls | grep main`
- `cat < input.txt`
- `echo hello > out.txt`
- `sleep 5 &` (백그라운드 실행)


## 🔹 3단계: 사용성 개선 및 고급 기능 (C 계속)

### 🎯 목표
- 환경변수, 프롬프트 꾸미기, 설정파일(`.myshellrc`) 처리
- SIGINT 핸들링, 히스토리 저장 등 사용성 개선

### 🧩 기술 키워드
- `getenv`, `putenv`, `signal`
- `readline`, `getline` (입력 개선)
- `fopen`, 파일 읽기 (`.myshellrc`, `.myshell_history`)

### 🛠 과제
- 프롬프트에 현재 디렉토리 표시
- `export`, `unset` 명령어 구현
- 입력 자동 완성 (선택 과제)

---