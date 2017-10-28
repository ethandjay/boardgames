========================================================================
    CONSOLE APPLICATION : lab4 Project Overview for Ethan Jaynes
========================================================================

AppWizard has created this lab4 application for you.

This file contains a summary of what you will find in each of the files that
make up your lab4 application.


lab4.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

lab4.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

lab4.cpp
    This is the main application source file.

Saved File Format:
    First line is game name, second line is turn count or NODATA if there is not prior saved game. In Reversi, the third line is
    whoever's turn it was when the game last quit. The next lines are in the format color (int), i position, j position.


Errors and Warnings:

Having to change int to size_t where appropriate
Potential move checker didn't account for situations in which a path contained no pieces
Cases fellthrough because I forgot breaks
Capitalizing the direction enums to prevent ambiguity
Many situations in which I only changed the color of a piece and not it's display or name
Forgot to account for the situation in which on player is unable to move but the other one can
Declaring non-virtual "winner" in Reversi class prevented it from being called from the base class
Checking if the board is full by comparing all pieces to the piece at (0,0) failed when there wasn't a piece at (0,0)
Accounting for the many situations in which enums are automatically casted as ints and the many cases in which they aren't


Test Cases:

Bad inputs in saved file - Failed at first (issues appending ints to strings and doing bounds checks)
			 - Assumes saved txt file already exists, otherwise throw stream_fail error (3)
			 - Assumes users will pick new name each time Reversi is run
			 - Test succeeded
Testing invalid inputs in all states (Coordinate is off the board, piece already exists, piece has no valid move)
			 - Success, reprompts every time
Testing malformed inputs in all states ("1,,2", "5,shidal")
			 - Success, reprompts every time
User can quit from any state - Success, retains turn count even when in move continuation (Almonds case)
Testing usage function - Success, all bad inputs (too many, too few arguments, misspelled game names) print 
			"lab4.exe [NineAlmonds, MagicSquare, Reversi <Player1> <Player2>]" and return wrong_args error level (2)
Stalemate test - Success, returns stalemate error level (7)
	       - Stalemate checks AFTER each move, so rigging a stalemate in the save txt actually didn't work well as a test.
		 Save txts were rigged so that stalemate was one move away. A situation in which the game loads a saved file
		 already in stalemate is logically impossible if the game is played from the console.
Winning (Done) test - Success, prints out determined winner and returns success code (0)


Copy Control:

Copy(-assignment) constructor:
	Both of these features are disabled in this project - each one is set to delete in the base class header declaration so
	the complier won't synthesize them. Because the game itself is a static instance variable of the game base class, only one
	instance of it should be allowed to exist at once. Allowing copy constructing and copy assignment constructing could
	create a situation in which two separate instances of the game exist, and this is not allowed by design.

Move(-assignment) operator:
	Both of these features are disabled in this project - each one is set to delete in the base class header declaration so
	the complier won't synthesize them. This is also due to the fact that the base classes's static instance of the game must
	be singluar. Allowing move operations would open up the possibility of having an instance of the game existing outside
	of the static shared pointer, which, in this case, would be null (after the original game instance deletes itself).

Destructor:
	All destructors were allowed to synthesize by virtue of the fact that they were not told specifically not to synthesize.
	There is no danger in destroying a static instance, as it is perfectly legal for that instance to be null. And as long
	as all derived classes also have their destructors synthesized, the burndown from the derived class up the the derivative
	class should be a smooth one.
	



/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named lab4.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
