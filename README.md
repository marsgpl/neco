# NeCo - Neuralink Compression Challenge solution

Author: Iurii Belobeev <marsgpl@gmail.com>

## init

```sh
wget https://content.neuralink.com/compression-challenge/eval.sh
wget https://content.neuralink.com/compression-challenge/data.zip
tar -xvf data.zip
```

## build

```sh
cd src && make && cp encode decode ..
cd ..
```

## notes

```sh
files: 743
total size: 146800526
avg size: 197578
total points n: 73383917
avg points n: 98767
uniq points n: 637
uniq deltas n: 11106
```
