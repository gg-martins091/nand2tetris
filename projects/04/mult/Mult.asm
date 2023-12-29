	@2
	M=0

	@0
	D=M
	@END
	D;JEQ

	@1
	D=M
	@END
	D;JEQ

(LOOP)
	@1
	D=M
	@END
	D;JEQ

	@1
	M=M-1

	@0
	D=M
	@2
	M=M+D

	@LOOP
	0;JMP

(END)
	@END
	0;JMP