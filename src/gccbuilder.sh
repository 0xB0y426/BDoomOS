#!/bin/bash

# Defina as variáveis de ambiente
PREFIX="$HOME/opt/cross"
TARGET=i686-elf
PATH="$PREFIX/bin:$PATH"
BINUTILS_VERSION=2.40
GCC_VERSION=12.2.0

# Função para exibir mensagens formatadas
function echo_info {
  echo -e "\e[34m[INFO]\e[0m $1"
}

# Criar diretórios necessários
mkdir -p $PREFIX
mkdir -p ~/cross-src
cd ~/cross-src

# Instalar dependências
echo_info "Instalando dependências..."
sudo apt update
sudo apt install -y build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo libisl-dev wget

# Baixar e compilar binutils
echo_info "Baixando binutils-$BINUTILS_VERSION..."
wget http://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VERSION.tar.gz
tar -xvzf binutils-$BINUTILS_VERSION.tar.gz
cd binutils-$BINUTILS_VERSION
mkdir build && cd build

echo_info "Compilando binutils..."
../configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make -j$(nproc)
make install
cd ~/cross-src

# Baixar e compilar GCC
echo_info "Baixando gcc-$GCC_VERSION..."
wget http://ftp.gnu.org/gnu/gcc/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.gz
tar -xvzf gcc-$GCC_VERSION.tar.gz
cd gcc-$GCC_VERSION
mkdir build && cd build

echo_info "Compilando GCC..."
../configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc -j$(nproc)
make all-target-libgcc -j$(nproc)
make install-gcc
make install-target-libgcc

echo_info "Compilação concluída!"
echo_info "Adicione o caminho ao PATH: export PATH=\"$PREFIX/bin:\$PATH\""
