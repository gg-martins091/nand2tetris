@SCREEN
D=A
@SC
M=D
@KD
M=0

(LOOP)
	
	// IF kbd pressed, goto write
	@KBD
	D=M
	@WRITE
	D;JGT

	// If not pressed, then
	// if current pixel written > screen base goto clear
	@SCREEN
	D=A
	@SC
	D=M-D
	@CLEAR
	D;JGT

	// else do nothing
	@LOOP
	0;JMP

	(WRITE)
		@SC
		A=M
		M=-1 		// write 1111111111111111 to current screen pointer
		@SC
		M=M+1 		// increment current screen pointer

		@KBD
		D=M
		@CLEAR
		D;JEQ       // goto clear if kbd not pressed

		@WRITE      // keep writing
		0;JMP

	(CLEAR)
		@SC
		A=M
		M=0			// write 0 to current screen pointer

		@SCREEN
		D=A
		@SC
		D=M-D		
		@LOOP
		D;JLE		// if current screen pointer == base screen, goto loop
		@SC
		M=M-1		// else decrement current screen pointer

		@KBD 		
		D=M
		@WRITE
		D;JGT     	// go back to write if kbd is pressed, this could be handled in a better (prettier) way, but works.

		@CLEAR
		0;JMP







