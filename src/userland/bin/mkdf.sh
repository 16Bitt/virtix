#!/bin/sh

set -e

target=$1
image=$2
count=0

if ! [ -d $target ]; then
	echo mkdf needs a valid directory to create
	exit -1
fi

if ! [ -a $image ]; then
	echo mkdf needs a valid image to create vfs
fi

image=$(readlink -f $image)
target=$(readlink -f $target)
cd $target

inc_count () {
	let "count += 1"
}

init_dir () {
	touch $1.DIR
	echo DEEPFAT > $1.DIR
	echo DEEPDIR >> $1.DIR
}

add_record () {
	echo $2 $3 $4 >> $1.DIR
}

dinit_dir () {
	echo END >> $1.DIR
}

mkdf_dir () {
	init_dir $2
	local tpath=$(readlink -f $1)
	local tfile=$(readlink -f $2)
	local parent=$(readlink -f $3)
	cd $tpath
	
	for i in *; do
		echo Packaging $i...
		
		if [ -d $i ]; then
			if [ "${i##*.}" = DIR ]; then
				continue
			fi

			inc_count
			add_record $tfile DIR $i $count.DIR
			local hold=$count.DIR
			mkdf_dir $i $count $hold
		else
			if [ $i = hdd.img ]; then
				continue
			fi

			inc_count
			add_record $tfile FIL $i $count.FIL
			mcopy -i $image $i ::/$count.FIL
		fi
	done

	cd -
}

mkdf_dir $target DFATROOT DFATROOT.DIR

cd $target
for i in *.DIR; do
	echo ENDDIR >> $i
done

mcopy -i $image *.DIR ::/
rm *.DIR
echo deepFAT loaded.
