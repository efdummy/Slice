**SAMPLE SESSION**

Suppose you want to send a 27 Mb video (mybigvideo.wmv) to a friend by e-mail but you know that your e-mail provider blocks e-mails that are bigger than 5 Mb. Just slice your video in smaller chunks !
You will then have to send Slice.exe utility to your recipient so that he will be able to rebuild the original file from the sliced chunks.
Here is a sample session, using slice :

C:\Temp>dir mybigvideo.wmv

01/06/2011  17:43           27 313 379 mybigvideo.wmv
               1 file(s)          27 313 379 bytes

C:\Temp>slice mybigvideo.wmv

I'm slicing mybigvideo.wmv in 5000000 bytes chunks, please wait...
Slice mybigvideo.wmv.chunk.1...
Slice mybigvideo.wmv.chunk.2...
Slice mybigvideo.wmv.chunk.3...
Slice mybigvideo.wmv.chunk.4...
Slice mybigvideo.wmv.chunk.5...
Slice mybigvideo.wmv.chunk.6...
Everything sliced (6 chunks & 27313379 bytes).

C:\Temp>del mybigvideo.wmv

C:\Temp>dir mybigvideo.wmv

File not found

C:\Temp>slice mybigvideo.wmv.chunk.1

I'm unslicing mybigvideo.wmv.chunk.1, please wait...
Slice mybigvideo.wmv.chunk.1...
Slice mybigvideo.wmv.chunk.2...
Slice mybigvideo.wmv.chunk.3...
Slice mybigvideo.wmv.chunk.4...
Slice mybigvideo.wmv.chunk.5...
Slice mybigvideo.wmv.chunk.6...
Everything unsliced (27313379 bytes).

C:\Temp>dir mybigvideo.wmv

01/06/2011  17:43           27 313 379 mybigvideo.wmv
               1 file(s)          27 313 379 bytes

**SETTING CHUNKS' SIZE**

If you want to cut the file in bigger chunks, just use the optional size parameter.
In the following sample, the chunks will be 8 Mb sized :

C:\Temp>slice mybigvideo.wmv 8000000

**PRESERVING FILE'S ATTRIBUTES**

Slice.exe preserves file's last access and last write dates and times.