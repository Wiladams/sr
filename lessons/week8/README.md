Week 8 - Accelerated development

Now that we've developed some basic tools from simple 
data structures, to collections, to graphics primitives,
it's time to start putting it all together to create some
interesting applications.

There are some more standard tools to create though.  Being
able to read in graphics images is one key tool which will 
make for some interesting UI.  Having the ability to read 
any files in, and treat them as streams of various kinds of
numeric values will be hugely valuable as well.

One cornerstone of dealing with data is the BinStream.  This 
object allows you to easily ready and write numeric values, 
strings, and arrays of bytes from a stream.  This will be used
for decoding image file formats.

Memory mapped files are represented in the mmap.hpp file.  This
allows you to take any file, no matter how large, and treat it 
like is simply a pointer in memory.  This is great when used in 
conjunction with the BinStream as the file can be easily read.

QueIterator has been improved upon, where filters can be chained
together.

As there are more windows APIs used, there has been some organization 
of the header files.  All Windows specific routines are located
in the files beginning with 'win32_'.