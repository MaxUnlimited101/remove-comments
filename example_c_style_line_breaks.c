line 1
line 2 //should not be seen
line 3/* inline comment here */ 
line 4/* multiline
	comment
	here */
line 5 comment with a break here: // should not\
	this should be seen
line 6 after line comment with a break
line 7// comment 
line 8/* comment */
line 9/*
	comment \
	comment
	// comment
*/
line 10 (should not work)// /* \
*/ comment\
comment */
line 11 (should not work properly) // comment \
	comment \
	comment	
line 12 (should work)/*
	comment
	// comment
	\
	comment
*/
line 13
line 14 // /*inline*/ line
line 15 /*inline //*/ 