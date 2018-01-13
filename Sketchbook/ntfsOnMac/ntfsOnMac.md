# 第一次尝试 #

![1](1.jpg)

![2](2.jpg)

![3](3.jpg)

![4](4.jpg)

# 第二次尝试 #

![5](5.jpg)

![6](6.jpg)

# 第三次尝试 #

```shell
cd /Volumes
sudo mkdir TmpPictures
diskutil list
diskutil unmount /dev/disk2s2
sudo mount -w -t ntfs -o rw,nobrowse /dev/disk2s2 /Volumes/TmpPictures
open TmpPictures
```

