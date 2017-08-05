#!/usr/bin/env bash

export CTEST_PARALLEL_LEVEL=1
export CCACHE_TEMPDIR=/tmp
ccache -M 500M
ccache -z

cd /root/QGIS

mkdir -p build-docker &&

pushd build-docker

cmake \
 -GNinja \
 -DWITH_STAGED_PLUGINS=ON \
 -DWITH_GRASS=OFF \
 -DSUPPRESS_QT_WARNINGS=ON \
 -DENABLE_MODELTEST=ON \
 -DENABLE_PGTEST=ON \
 -DWITH_QSPATIALITE=ON \
 -DWITH_QWTPOLAR=OFF \
 -DWITH_APIDOC=OFF \
 -DWITH_ASTYLE=OFF \
 -DWITH_DESKTOP=ON \
 -DWITH_BINDINGS=ON \
 -DDISABLE_DEPRECATED=ON \
 -DCXX_EXTRA_FLAGS=${CLANG_WARNINGS} ..

ninja

popd

printf "[qgis_test]\nhost=postgres\nport=5432\ndbname=docker\nuser=docker\npassword=docker" > ~/.pg_service.conf
export PGUSER=docker
/root/QGIS/tests/testdata/provider/testdata_pg.sh

python /root/QGIS/.ci/travis/scripts/ctest2travis.py \
  xvfb-run ctest -V -E "$(cat /root/QGIS/.ci/travis/linux/blacklist.txt | sed -r '/^(#.*?)?$/d' | paste -sd '|' -)" -S /root/QGIS/.ci/travis/travis.ctest --output-on-failure

ccache -s

[ -r /tmp/ctest-important.log ] && cat /tmp/ctest-important.log
