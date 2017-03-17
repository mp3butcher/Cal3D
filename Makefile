DRY=--dry-run

PACKAGING_FARM_LIBDIR=/home/loic/local/ports/packaging-farm/packaging-farm/lib

download:
	rsync -avH rsync://download.gna.org/download/cal3d/sources/ sources/

upload:
	find . -type d | xargs chmod g+w
	find . -type d | xargs chmod g+s
	rsync ${DRY} --delete --delete-excluded --exclude='/farms' --exclude '*.tmp' --exclude='*~' -avH -z * ./ download.gna.org:/upload/cal3d/

dupmerge:
	${PACKAGING_FARM_LIBDIR}/dupmerge.sh ${DRY} .

#FARM_HOST=rsync://hploic
FARM_HOST=root@hploic.lan:/var/lib
PACKAGES=cal3d

sync: $(patsubst %,sync/%,${PACKAGES})
	rsync ${DRY} --delete -avH -z ${FARM_HOST}/packaging-farm/html/ html/

$(patsubst %,sync/%,${PACKAGES}):
	rsync ${DRY} --delete -avH -z ${FARM_HOST}/packaging-farm/${@F}/ ${@F}/

archive archives: $(patsubst %,archives/%,${PACKAGES})

$(patsubst %,archives/%,${PACKAGES})::
	rsync ${DRY} -avH ${@F}/ archives/${@F}/
	${MAKE} official/${@F}

.PHONY: $(patsubst %,archives/%,${PACKAGES})

define OFFICIAL
official/${1}:: $(patsubst %,official/${1}/archives/%,${2})

$(patsubst %,official/${1}/archives/%,${2})::
	cd archives/${1}/*/*/$${@F} && mkdir -p official && cd official && ${PACKAGING_FARM_LIBDIR}/$$$$(packaging-farm --var DISTRIBUTION=$${@F} DISTTYPE)/official-op.sh -d $${@F} archive ${1}
endef

$(eval $(call OFFICIAL,cal3d,unstable dapper))

sign: $(patsubst %,sign/%,${PACKAGES})

$(patsubst %,sign/%,${PACKAGES}):
	debsign ${@F}/gnulinux/debian/unstable/src/*.changes

