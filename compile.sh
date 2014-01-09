#!/usr/bin/env bash
## Usage instructions:
## yum/apt-get install git
## git clone https://github.com/otland/forgottenserver
## cd forgottenserver
## bash ncompile.sh
## It is important to run with 'bash' and not 'sh' or as an executable(./file)
## because the script is multi-platform so I did not include the shebang
## and old shell does not support a few things I did here

## Script by dominique120
## Few edits and fixes by fallen(decltype)
## Idea from the compile.sh script that was packed with some TFS 0.4 revs.
## Made for TFS 1.0
## If you plan of editing try to keep the structure and conventions
## Line endings must be LF only, not CRLF.

## TODO:
## Add other operating systems (MacOSX, Arch Linux)
## Test the multicore build section 
## Add other optimizations to the make process (maybe use ccache?)
## Make it prettier?

## Get CPU core count
## Still testing
cpuCores=$(nproc)
## make processes to be spawned.
coreBuild=$((cpuCores + 1))

## Colors
greenText=$(tput setab 0; tput setaf 2)
redText=$(tput setab 0; tput setaf 1)
blueText=$(tput setab 0; tput setaf 6)
yellowText=$(tput setab 0; tput setaf 3)
none=$(tput sgr0)
###
### Functions to simplify stuff :)
###

debianDeps() {
	apt-get -y install cmake build-essential liblua5.2-dev \
		libgmp3-dev libmysqlclient-dev libboost-system-dev
}

fedoraDeps() {
	yum -y install cmake gcc-c++ boost-devel \
		gmp-devel community-mysql-devel lua-devel
}

centDeps() {
	yum -y install cmake gcc-c++ boost-devel \
		gmp-devel mysql-devel lua-devel
}

bsdDeps() {
	cd /usr/ports/shells/bash && make install clean BATCH=yes
	cd /usr/ports/devel/git && make install clean BATCH=yes
	cd /usr/ports/devel/cmake && make install clean BATCH=yes
	cd /usr/ports/lang/gcc47 && make install clean BATCH=yes
	cd /usr/ports/lang/luajit && make install clean BATCH=yes
	cd /usr/ports/devel/boost-libs && make install clean BATCH=yes
	cd /usr/ports/math/gmp && make install clean BATCH=yes
	cd /usr/ports/databases/mysql-connector-c && make install clean BATCH=yes
}

libInstall() {
	echo "Libraries and Build Tools... Installed"
}

bsdBuild() {
	echo "Building on FreeBSD"
	mkdir build && cd build
	CXX=g++47 cmake ..
	echo "Build on $cpuCores threads with $coreBuild processes? (experimental but loads faster) y or n "
		read ans1_4
			if [[ $ans1_4 = "y" ]]; then
				echo -e $greenText"Building on $cpuCores threads with $coreBuild processes."$none
				make -j $coreBuild
			elif [[ $ans1_4 = "n" ]]; then
				echo -e $blueText"Building on a single thread."$none
				make
			else
				echo -e $redText"Answer y or n"$none
			fi
}	

genBuild() {
	echo "Building..."
	mkdir build && cd build
	cmake ..
		echo "Build on $cpuCores threads with $coreBuild processes? (experimental but loads faster) y or n "
		read ans1_4
			if [[ $ans1_4 = "y" ]]; then
				echo -e $greenText"Building on $cpuCores threads with $coreBuild processes."$none
				make -j $coreBuild
			elif [[ $ans1_4 = "n" ]]; then
				echo -e $blueText"Building on a single thread."$none
				make
			else
				echo -e $redText"Answer y or n"$none
			fi				
}
# Unused clean, I'm leaving it here in case we might need it in the future.
clean() {
	mkdir objs/
	mv *.o objs/
	echo "There might be a few leftover files."
}

###
### Script starts here
###

#check if root
if [[ $EUID -ne 0 ]]; then
	echo "You must be root to use this script, press enter to exit."
	read end
	exit 1
fi
#OS dependencies and other stuff
echo "Chose your Operating System. {Supported OS: Debian, Ubuntu, Fedora, FreeBSD."$yellowText" Not recommended->"$none " CentOS or Scientific Linux"
#Note for SL6 and CentOS, look below.
read ans1 
			
if [[ $ans1 = "Fedora" ]]; then
	echo -n "Should the script install dependencies? y or n: "
	read ans1_1
	if [[ $ans1_1 = "y" ]]; then
		fedoraDeps
	elif [[ $ans1_1 = "n" ]]; then
		:
	else
		echo "Answer 'y' or 'n' "
	fi
elif [[ $ans1 = "CentOS" ]] || [[ $ans1 = "Scientific Linux" ]]; then
	echo -n "Should the script install dependencies? y or n: " $redText"TFS 1.0 developers discourage the use of these distributions because they have outdated packages."$none $blueText"*NOTE that versions of cmake and boost installed in this process are old, they should be installed manually by adding repositories or compiling them manually."$none
	read ans1_1
	if [[ $ans1_1 = "y" ]]; then
		centDeps
	elif [[ $ans1_1 = "n" ]]; then
		:
	else
		echo "Answer 'y' or 'n' "
	fi	
elif [[ $ans1 = "Debian" ]] || [[ $ans1 = "Ubuntu" ]]; then
	echo -n "Should the script install dependencies? y or n: "
	read ans1_1
	if [[ $ans1_1 = "y" ]]; then
		debianDeps
	elif [[ $ans1_1 = "n" ]]; then
		:
	else
		echo "Answer 'y' or 'n' "
	fi
elif [[ $ans1 = "FreeBSD" ]]; then
	echo -n "Should the script install dependencies? y or n: "
	read ans1_1
		if [[ $ans1_1 = "y" ]]; then
			bsdDeps
		elif [[ $ans1_1 = "n" ]]; then
			:
		else
			echo "Answer 'y' or 'n' "
		fi				
		else
			echo "Pick a valid OS"		
		fi
		
#Compiling here
echo -n "Are we on FreeBSD? y or n: "
read ans1_2
	if [[ $ans1_2 = "y" ]]; then
		bsdbuild
	elif [[ $ans1_2 = "n" ]]; then
		genBuild
	else
		echo "Answer y or n"
	fi

## Notes
# 1: SL6 and CentOS latest version of CMAKE is 2.6.4 as of 1/7/2014(us), thus incompatible with TFS 1.0 building process. It must be manually added, tutorial below:
# https://github.com/jackywei/HOW-TO-BUILD-HHVM-WiKi/wiki/Build-&-Install-CMake-2.8.10.2-in-CentOS6.3
# And look at issue #391 for more details about old packages in SL6 and CentOS