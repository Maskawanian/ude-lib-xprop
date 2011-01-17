# Maintainer: Maskawanian <aur@cryingwolf.org>
pkgname=ude-lib-xprop
pkgver=2009.10.12
pkgrel=1
pkgdesc="LD_PRELOAD hack to add _NET_WM_PID and WM_CLIENT_MACHINE to applications which foolishly do not impliment Extended Window Manager Hints."
arch=(x86_64 i386)
license=('BSD')
url="http://code.google.com/p/powerview-xprop/"
depends=()
provides=(powerview-xprop-hack)
conflicts=(powerview-xprop-hack)
source=(
	build.sh
	xprop.h
	xprop.c
)
md5sums=('c29cc35c88dde21da6dcaebc24d3e3c8'
         'b0428ee3ba45213ee82c35722761fa6d'
         '6a066e2f835de0705280e28338fbe336')


build() {
	cd ${srcdir} || return 1
	sh build.sh || return 1
	
	mkdir -p ${pkgdir}/usr/share/ude/xprop/ || return 1
	cp ${srcdir}/ude-lib-xprop.so ${pkgdir}/usr/share/ude/xprop/ude-lib-xprop.so || return 1
	chmod +x ${pkgdir}/usr/share/ude/xprop/ude-lib-xprop.so || return 1
}
