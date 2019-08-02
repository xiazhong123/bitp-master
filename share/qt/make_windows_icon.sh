#!/bin/bash
# create multiresolution windows icon
#mainnet
ICON_SRC=../../src/qt/res/icons/bitpcoin.png
ICON_DST=../../src/qt/res/icons/bitpcoin.ico
convert ${ICON_SRC} -resize 16x16 bitpcoin-16.png
convert ${ICON_SRC} -resize 32x32 bitpcoin-32.png
convert ${ICON_SRC} -resize 48x48 bitpcoin-48.png
convert bitpcoin-16.png bitpcoin-32.png bitpcoin-48.png ${ICON_DST}
#testnet
ICON_SRC=../../src/qt/res/icons/bitpcoin_testnet.png
ICON_DST=../../src/qt/res/icons/bitpcoin_testnet.ico
convert ${ICON_SRC} -resize 16x16 bitpcoin-16.png
convert ${ICON_SRC} -resize 32x32 bitpcoin-32.png
convert ${ICON_SRC} -resize 48x48 bitpcoin-48.png
convert bitpcoin-16.png bitpcoin-32.png bitpcoin-48.png ${ICON_DST}
rm bitpcoin-16.png bitpcoin-32.png bitpcoin-48.png
