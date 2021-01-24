<pre>
********      *     **           *     ********* ******** ******** *********
********     ***    **          ***    ********* ******** ******** *********
**          ** **   **         ** **   **           **       **    **
**   ***   **   **  **        **   **  **           **       **    *********
**    **  ********* **       ********* **           **       **    *********
**    **  ********* **       **     ** **           **       **           ** ****   ***
********  **     ** ******** **     ** *********    **    ******** ********* *   *  *  *
********  **     ** ******** **     ** *********    **    ******** *********     *  *   *
                                                                               **   *   *
                                                                              **    *  *
                                                                             *****  ***
</pre>
Video of Game from 2009 Upload: https://www.youtube.com/watch?v=MV10MHOrAHI

When adding existing files to your project, if you recieve errors
you should make sure your in debug mode and/or you have to set
"Character Set" to "Use Multi-byte character set", I added code in
the game.h file so you don't have to load the library files in linker.

If for some reason it isn't working on your add these to your linker
settings.

d3d9.lib
d3dx9.lib
dxguid.lib
dxerr9.lib
dsound.lib
dinput8.lib
winmm.lib