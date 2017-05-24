#!/usr/bin/env bash

## Usage instructions:
## yum/apt-get install git
## git clone https://github.com/otland/forgottenserver
## cd forgottenserver
## bash compile.sh
## The shebang may or may not work with the systems that the script can work on 
## so just use "bash compile.sh" instead of ./comile.sh

## In FreeBSD the user must first install bash to be able to execute this script
## This command can be used:
## cd /usr/ports/shells/bash && make install clean BATCH=yes
## The actual dependency install process also takes ages :/

## Script by dominique120
## Few edits and fixes by fallen(decltype)
## Idea came from the compile.sh script that was packed with some TFS 0.4 revs.
## Made for TFS 1.0
## If you plan of editing try to keep the structure and conventions
## Line endings must be LF only, not CRLF.

## TODO:
## Add other operating systems (MacOSX, Arch Linux)
## Test the multicore build section on other systems
## Add other optimizations to the make process faster(maybe use ccache?)
## Make it prettier?

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
	libInstall
}

fedoraDeps() {
	yum -y install cmake gcc-c++ boost-devel \
		gmp-devel mysql-devel lua-devel
	libInstall
}

centDeps() {
	yum -y install cmake gcc-c++ boost-devel \
		gmp-devel mysql-devel lua-devel
	libInstall
}

bsdDeps() {
	cd /usr/ports/devel/cmake && make install clean BATCH=yes
	cd /usr/ports/lang/gcc47 && make install clean BATCH=yes
	cd /usr/ports/lang/luajit && make install clean BATCH=yes
	cd /usr/ports/devel/boost-libs && make install clean BATCH=yes
	cd /usr/ports/math/gmp && make install clean BATCH=yes
	cd /usr/ports/databases/mysql-connector-c && make install clean BATCH=yes
	libInstall
}

archDeps() {
	pacman -Syu
	pacman -S --noconfirm base-devel git cmake lua gmp boost boost-libs libmariadbclient
}

macDeps() {
	brew install git cmake gmp mysql-connector-c lua52
	brew install boost --with-c++11
}

hombrew() {
	echo -n "Homebrew is required to install the required packages, should the script install it? If its installed enter 'n' [y or n]:"
	read ansmac2
		if [[ $ansmac2 = "y" ]]; then
			ruby -e "$(curl -fsSL https://raw.github.com/mxcl/homebrew/go)"
		elif [[ $ansmac2 = "n" ]]; then
			:
		else
			echo "answer 'y' or 'n'"
		fi
}

libInstall() {
	echo $greenText"Libraries and Build Tools... Installed"$none
}

bsdBuild() {
## Fetch number of CPUs
cpuCores=$(sysctl -a | egrep -i 'hw.ncpu')
## make processes to be spawned.
coreBuild=$((cpuCores + 1))

	echo "Building on FreeBSD"
	mkdir build && cd build
	CXX=g++47 cmake ..
	echo "Build on $cpuCores threads with $coreBuild processes? (experimental but builds faster) [y or n]:"
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
## Fetch number of CPUs
cpuCores=$(nproc)
## make processes to be spawned.
coreBuild=$((cpuCores + 1))

	echo "Building..."
	mkdir build && cd build
	cmake ..
		echo "Build on $cpuCores threads with $coreBuild processes? (experimental but builds faster) [y or n]:"
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
# Clean used before compile to remove the build directory in case there has been a previous build
clean() {
	rm -R build
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
echo "Chose your Operating System. {Supported: Debian, Ubuntu, Fedora, FreeBSD, ArchLinux, MacOSX."$yellowText"Not recommended->"$none "CentOS or Scientific Linux"
#Note for SL6 and CentOS, look below.
read ans1 
			
if [[ $ans1 = "Fedora" ]]; then
	echo -n "Should the script install dependencies? [y or n]:"
	read ans1_1
	if [[ $ans1_1 = "y" ]]; then
		fedoraDeps
	elif [[ $ans1_1 = "n" ]]; then
		echo "Continuing..."
		:
	else
		echo "Answer 'y' or 'n' "
	fi
elif [[ $ans1 = "CentOS" ]] || [[ $ans1 = "Scientific Linux" ]]; then
	echo -n $redText"TFS 1.0 developers discourage the use of these distributions because they have" \
		"outdated packages."$none $blueText"*NOTE that versions of cmake and boost installed in this process are old, they should be installed manually by" \ 
		"adding repositories or compiling them manually."$none "Should the script install dependencies? [y or n]:"
	read ans1_1
	if [[ $ans1_1 = "y" ]]; then
		centDeps
	elif [[ $ans1_1 = "n" ]]; then
		echo "Continuing..."
		:
	else
		echo "Answer 'y' or 'n' "
	fi	
elif [[ $ans1 = "Debian" ]] || [[ $ans1 = "Ubuntu" ]]; then
	echo -n "Should the script install dependencies? [y or n]:"
	read ans1_1
	if [[ $ans1_1 = "y" ]]; then
		debianDeps
	elif [[ $ans1_1 = "n" ]]; then
		echo "Continuing..."
		:
	else
		echo "Answer 'y' or 'n' "
	fi
elif [[ $ans1 = "FreeBSD" ]]; then
	echo -n "Should the script install dependencies? [y or n]:"
	read ans1_1
		if [[ $ans1_1 = "y" ]]; then
			bsdDeps
		elif [[ $ans1_1 = "n" ]]; then
			:
		else
			echo "Answer 'y' or 'n' "
		fi	
elif [[ $ans1 = "ArchLinux" ]]; then
	echo -n "Should the script install dependencies? [y or n]:"
	read ans1_1
		if [[ $ans1_1 = "y" ]]; then
			archDeps
		elif [[ $ans1_1 = "n" ]]; then
			:
		else
			echo "Answer 'y' or 'n' "
		fi	
elif [[ $ans1 = "MacOSX" ]]; then
	echo -n "Should the script install dependencies? [y or n]:"
	read ansmac1
		if [[ $ansmac1 = "y" ]]; then
			homebrew
			macDeps
		elif [[ $ansmac1 = "n" ]]; then
			:
		else
			echo "Answer 'y' or 'n' "
		fi		
else
	echo "Pick a valid OS, options are case sensitive."	
	:			
	fi
		
		
#Clean
echo -n "Should the script clean the directory for any previous builds? [y or n]"
read ans1_6
	if [[ $ans1_6 = "y" ]]; then
		clean		
	elif [[ $ans1_6 = "n" ]]; then
		echo "Continuing..."
		:		
	else
		echo "Answer 'y' or 'n'"		
	fi

#Compiling here
echo -n "Are we on FreeBSD? y or n: "
read ans1_2
	if [[ $ans1_2 = "y" ]]; then
		echo "Make sure to have the latest version of gcc(4.8+) or add C++11 flags(4.7) or the compilation will fail."
		echo "Have they been installed? y or n: "
		read ans1_5
			if [[ $ans1_5 = "y" ]]; then
				bsdBuild
			elif [[ $ans1_5 = "n" ]]; then
				echo "Install those packages and re-run this script."
				:
			else
				echo "Answer 'y' or 'n'"
			fi
	elif [[ $ans1_2 = "n" ]]; then
		genBuild
	else
		echo "Answer y or n"
	fi

## Notes
# 1: SL6 and CentOS latest version of CMAKE is 2.6.4 as of 1/7/2014(us), thus incompatible with TFS 1.0 building process. It must be manually added, tutorial below:
# https://github.com/jackywei/HOW-TO-BUILD-HHVM-WiKi/wiki/Build-&-Install-CMake-2.8.10.2-in-CentOS6.3
# And look at issue #391 for more details about old packages in SL6 and CentOS