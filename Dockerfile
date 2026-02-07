# alibrary-illusion 빌드 환경 Dockerfile
# Node.js, Emscripten, CMake를 포함한 빌드 환경

FROM node:20-slim

# 빌드 도구 및 필수 패키지 설치
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    python3 \
    python3-pip \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Emscripten SDK 설치
ENV EMSDK_VERSION=3.1.50
ENV EMSDK_PATH=/opt/emscripten
WORKDIR /opt

RUN git clone https://github.com/emscripten-core/emsdk.git && \
    cd emsdk && \
    ./emsdk install ${EMSDK_VERSION} && \
    ./emsdk activate ${EMSDK_VERSION} && \
    rm -rf .git

# Emscripten 환경 변수 설정
ENV PATH="/opt/emsdk:/opt/emsdk/upstream/emscripten:/opt/emsdk/node/${EMSDK_VERSION}_64bit/bin:${PATH}"
ENV EMSDK="/opt/emsdk"
ENV EM_CONFIG="/opt/emsdk/.emscripten"
ENV EMSDK_NODE="/opt/emsdk/node/${EMSDK_VERSION}_64bit/bin/node"

# pnpm 설치
RUN npm install -g pnpm@latest

# 작업 디렉토리 설정
WORKDIR /workspace

# package.json 및 pnpm-lock.yaml 복사 (의존성 캐싱 최적화)
COPY package.json pnpm-lock.yaml* pnpm-workspace.yaml* ./
COPY packages/ ./packages/

# 의존성 설치
RUN pnpm install --frozen-lockfile || pnpm install

# 소스 코드 복사
COPY . .

# 기본 명령어
CMD ["/bin/bash"]
