# alibrary-illusion 빌드 자동화 Makefile

.PHONY: help install build build-ts build-wasm build-packages clean test docker-build docker-up docker-shell

# 기본 변수
NODE_ENV ?= development
BUILD_TYPE ?= Release
BINDING ?= MRUBY

# 디렉토리 경로
SRC_DIR := src
PACKAGES_DIR := packages
DIST_DIR := dist
BUILD_DIR := build

# 기본 타겟
.DEFAULT_GOAL := help

help: ## 도움말 표시
	@echo "alibrary-illusion 빌드 시스템"
	@echo ""
	@echo "사용 가능한 타겟:"
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "  %-20s %s\n", $$1, $$2}'

install: ## 의존성 설치
	@echo "📦 의존성 설치 중..."
	pnpm install

build: build-ts build-packages ## 전체 빌드 (TypeScript + npm 패키지)

build-ts: ## TypeScript 컴파일
	@echo "🔨 TypeScript 컴파일 중..."
	@mkdir -p $(DIST_DIR)
	@if [ -f tsconfig.json ]; then \
		pnpm exec tsc --build; \
	else \
		echo "⚠️  tsconfig.json이 없습니다. 패키지별로 빌드합니다."; \
		cd $(PACKAGES_DIR)/compiler && pnpm build || true; \
		cd $(PACKAGES_DIR)/compiler-zetpkg && pnpm build || true; \
	fi

build-wasm: ## Palcon-RGSS WASM 빌드
	@echo "🔨 Palcon-RGSS WASM 빌드 중..."
	@if command -v emcc >/dev/null 2>&1; then \
		cd $(SRC_DIR)/engine/Palcon-RGSS/build && \
		$(MAKE) build BINDING=$(BINDING) BUILD_TYPE=$(BUILD_TYPE); \
	else \
		echo "⚠️  Emscripten이 설치되어 있지 않습니다. Docker를 사용하세요: make docker-build"; \
	fi

build-packages: ## npm 패키지 빌드
	@echo "📦 npm 패키지 빌드 중..."
	@if [ -d $(PACKAGES_DIR)/compiler ]; then \
		cd $(PACKAGES_DIR)/compiler && pnpm build; \
	fi
	@if [ -d $(PACKAGES_DIR)/compiler-zetpkg ]; then \
		cd $(PACKAGES_DIR)/compiler-zetpkg && pnpm build; \
	fi

clean: ## 빌드 산출물 정리
	@echo "🧹 빌드 산출물 정리 중..."
	rm -rf $(DIST_DIR)
	rm -rf $(BUILD_DIR)
	rm -rf node_modules/.cache
	@if [ -d $(PACKAGES_DIR)/compiler ]; then \
		cd $(PACKAGES_DIR)/compiler && rm -rf dist node_modules/.cache || true; \
	fi
	@if [ -d $(PACKAGES_DIR)/compiler-zetpkg ]; then \
		cd $(PACKAGES_DIR)/compiler-zetpkg && rm -rf dist node_modules/.cache || true; \
	fi
	@if [ -d $(SRC_DIR)/engine/Palcon-RGSS/build ]; then \
		cd $(SRC_DIR)/engine/Palcon-RGSS/build && $(MAKE) clean || true; \
	fi

test: ## 테스트 실행
	@echo "🧪 테스트 실행 중..."
	@if [ -d $(PACKAGES_DIR)/compiler ]; then \
		cd $(PACKAGES_DIR)/compiler && pnpm test || true; \
	fi
	@if [ -d $(PACKAGES_DIR)/compiler-zetpkg ]; then \
		cd $(PACKAGES_DIR)/compiler-zetpkg && pnpm test || true; \
	fi

# Docker 관련 타겟
docker-build: ## Docker 이미지 빌드
	@echo "🐳 Docker 이미지 빌드 중..."
	docker compose build

docker-up: ## Docker 컨테이너 시작
	@echo "🐳 Docker 컨테이너 시작 중..."
	docker compose up -d

docker-shell: ## Docker 컨테이너 쉘 접속
	@echo "🐳 Docker 컨테이너 쉘 접속 중..."
	docker compose exec builder /bin/bash

docker-down: ## Docker 컨테이너 중지
	@echo "🐳 Docker 컨테이너 중지 중..."
	docker compose down

# 개발 모드
dev: ## 개발 모드 (watch)
	@echo "👀 개발 모드 시작..."
	@if [ -d $(PACKAGES_DIR)/compiler ]; then \
		cd $(PACKAGES_DIR)/compiler && pnpm dev & \
	fi
	@if [ -d $(PACKAGES_DIR)/compiler-zetpkg ]; then \
		cd $(PACKAGES_DIR)/compiler-zetpkg && pnpm dev & \
	fi
	@wait

# 린트 및 포맷
lint: ## 코드 린트
	@echo "🔍 코드 린트 중..."
	@if command -v eslint >/dev/null 2>&1; then \
		pnpm exec eslint "src/**/*.ts" "packages/**/*.ts" || true; \
	else \
		echo "⚠️  ESLint가 설치되어 있지 않습니다."; \
	fi

format: ## 코드 포맷
	@echo "✨ 코드 포맷 중..."
	@if command -v prettier >/dev/null 2>&1; then \
		pnpm exec prettier --write "src/**/*.ts" "packages/**/*.ts" || true; \
	else \
		echo "⚠️  Prettier가 설치되어 있지 않습니다."; \
	fi
