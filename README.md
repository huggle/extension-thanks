thanks
=========


This extensions give you some wikilove features and is designed for WMF wikis only

Note: This extension should be checked out in the folder named "huggle" in your huggle3-qt-lx repo

How to build
=============

On windows:
You need to build huggle core library first, once you have that, open the .pro file and ensure that include path is pointing to folder with huggle source code, so that all header files of huggle can be included. Copy huggle core library to src folder of extension.

Now try to build it. Produced .dll file needs to be copied to extensions folder

On linux:
Make sure that include folder points to huggle folder, then build the library and copy .so file to proper extensions path
