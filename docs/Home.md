**Project Description**
SLICE is a console exe that cuts your big files in chunks or rebuild them from the chunks. It's developed in C++. 
It makes it easier, for example, to send big files by e-mail when your provider blocks e-mails bigger than 10 Mo. You just have to slice your file and send the chunks.


**SLICE cut your big files in chunks or rebuild them from the chunks**

Syntax 1 to slice   : slice filename
Syntax 2 to slice   : slice filename size_of_chunks
Syntax 3 to unslice : slice filename.chunk.1
Default value for size_of_chunks is 5 Mo

To slice the 10 Mo bigvideo.mp4 file in two 5 Mo chunks:
C:\> slice bigvideo.mp4

To slice the 10 Mo bigvideo.mp4 file in five 2 Mo chunks:
C:\> slice bigvideo.mp4 2000000

To rebuild the bigvideo.mp4 from the chunks:
C:\> slice bigvideo.mp4.chunk.1

NB : all the chunks must be in the same folder