#Maintainer: Manjunatha Sarma Majety <manjunathasarmamajety@gmail.com>
pkgname=jrnlc
pkgver=4.0
pkgrel=1
pkgdesc="A tiny terminal journal for your secrets"
arch=('x86_64')
url="https://github.com/yourusername/jrnlc"
license=('MIT')
depends=('glibc')
makedepends=('cmake' 'gcc')
source=("$pkgname-$pkgver.tar.gz::https://github.com/manjunathamajety/jrnlc/archive/refs/tags/v4.0.tar.gz")
sha256sums=('SKIP')  # replace with actual SHA256

build() {
    cd "$srcdir/$pkgname-$pkgver"
    mkdir -p build
    cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release
    make
}

check() {
    cd "$srcdir/$pkgname-$pkgver/build"
    # ./run_tests  # optional, if you have Catch2 tests
}

package() {
    cd "$srcdir/$pkgname-$pkgver/build"
    make DESTDIR="$pkgdir" install
}

