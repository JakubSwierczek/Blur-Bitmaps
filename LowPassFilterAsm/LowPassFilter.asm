.CODE

MainProc PROC

	mov eax, 1
	ret

MainProc ENDP

;******************************
; *pixels_to_change	RCX
; *output_pixels		RDX
; width					R8
; fragment_height		R9
;******************************
LowPassFilterProc PROC
;*********************
	; RCX - loop counter
	; R10 - the remainder - pixel to change without xmm
	; R11 - input pixels
	; R12 - output
	;**********************
	push r10
	push r11
	push r12
	push r13
	push r15

	sub rsp, 16
	movdqu  xmmword ptr [rsp], xmm5
	sub rsp, 16
	movdqu  xmmword ptr [rsp], xmm6
	sub rsp, 16
	movdqu  xmmword ptr [rsp], xmm7
	sub rsp, 16
	movdqu  xmmword ptr [rsp], xmm8
	sub rsp, 16
	movdqu  xmmword ptr [rsp], xmm9
	sub rsp, 16
	movdqu  xmmword ptr [rsp], xmm10
	sub rsp, 16
	movdqu  xmmword ptr [rsp], xmm11
	sub rsp, 16
	movdqu  xmmword ptr [rsp], xmm12
	sub rsp, 16
	movdqu  xmmword ptr [rsp], xmm14
	sub rsp, 16
	movdqu  xmmword ptr [rsp], xmm15
		
	mov r11, rcx
	mov r12, rdx 
	mov rax, r8 ; pixels to change 
	mul r9
	mov r10, 8
	div r10 ; divide by 8 (number of pixels in one xmm)
	mov rcx, rax 
	mov r10, rdx

	;*********************************
	; R13 - line up 
	; r11 - current line
	; R14 - line below
	; R12 - output line
	;*********************************
	mov rax, r11
	sub rax, r8
	mov r13, rax

	mov rax, r11
	add rax, r8 
	mov r14, rax	
	
	; shift pixel
	mov rax, r13
	add rax, 1
	mov r13, rax

	mov rax, r11
	add rax, 1
	mov r11, rax

	mov rax, r14
	add rax, 1
	mov r14, rax

	mov rax, r12
	add rax, 1
	mov r12, rax

	;******************************
	; TEST WITHOUT XMM
	;mov r15, 9
	;mov rax, r8
	;mul r9
	;mov rcx, rax
	;jp LOOP_B
	;******************************

	; array with value to divider 
	;mov r15, offset arraymul ; multiplication instead of dividing
	mov r15, offset arraydiv
	vmovq xmm0, qword ptr [r15]
	vpmovzxbd xmm15, xmm0
	vpmovzxbd xmm14, xmm0

	vinsertf128 ymm15, ymm14, xmm15, 1
	vcvtdq2ps ymm15, ymm15 ; integer to float
	; VPSLLW xmm15, xmm15, 8 ; to multiplication

LOOP_A:
	vmovq xmm0, qword ptr [r13 - 1]
	vpmovzxbw xmm1, xmm0
	
	vmovq xmm0, qword ptr [r13]
	vpmovzxbw xmm2, xmm0

	vmovq xmm0, qword ptr [r13 + 1]
	vpmovzxbw xmm3, xmm0

	vmovq xmm0, qword ptr [r11 - 1]
	vpmovzxbw xmm4, xmm0

	vmovq xmm0, qword ptr [r11]
	vpmovzxbw xmm5, xmm0 

	vmovq xmm0, qword ptr [r11 + 1]
	vpmovzxbw xmm6, xmm0 
	 
	vmovq xmm0, qword ptr [r14 - 1]
	vpmovzxbw xmm7, xmm0

	vmovq xmm0, qword ptr [r14]
	vpmovzxbw xmm8, xmm0

	vmovq xmm0, qword ptr [r14 + 1]
	vpmovzxbw xmm9, xmm0

	; result in xmm10
	vmovdqa xmm10, xmm1
	vpaddd xmm10, xmm2, xmm10
	vpaddd xmm10, xmm3, xmm10
	
	vpaddd xmm10, xmm4, xmm10
	vpaddd xmm10, xmm5, xmm10
	vpaddd xmm10, xmm6, xmm10

	vpaddd xmm10, xmm7, xmm10
	vpaddd xmm10, xmm8, xmm10
	vpaddd xmm10, xmm9, xmm10

	;vpmulhuw xmm10, xmm10, xmm15

	
	;vxorps xmm0, xmm0, xmm0 ; xor xmm0 	
	;vpackuswb xmm10, xmm10, xmm0	
	
	vxorps xmm0, xmm0, xmm0 ; xor xmm0 
	vmovlhps xmm11, xmm10, xmm0 ; low 64 bits of xmm10 to low 64 bits xmm11
	vmovhlps xmm12, xmm0, xmm10 ; high 64 bits of xmm10 to low 64 bits xmm12
	nop
	vpmovzxwd xmm11, xmm11 ; 16 bits integer to 32 bits integer
	vpmovzxwd xmm12, xmm12 ; - || -
	vinsertf128 ymm10, ymm11, xmm12, 1 ; insert xmm12 to higher bits of ymm11 (in lower bits ymm11 is xmm11 so every numbers are in ymm10 aften this operation
	nop
	vcvtdq2ps ymm10, ymm10; integer to float
	
	vdivps ymm11, ymm10, ymm15 ; division

	nop
	vcvtps2dq ymm11, ymm11 ; float to integer


	vextractf128 xmm12, ymm11, 1

	vpackusdw xmm11, xmm11, xmm0 ; dword to word
	vpackusdw xmm12, xmm12, xmm0

	
	
	vmovlhps xmm10, xmm11, xmm12 ; low 64 bits of xmm12 to high 64 bits xmm10 and low 64 bits of xmm11 to low 64 bits xmm10
	vpackuswb xmm10, xmm10, xmm10; 
	nop
	
	; saving result in output
	vmovq qword ptr [r12], xmm10

	
	; shift pixels
	mov rax, r13
	add rax, 8
	mov r13, rax

	mov rax, r11
	add rax, 8
	mov r11, rax

	mov rax, r14
	add rax, 8
	mov r14, rax

	mov rax, r12
	add rax, 8
	mov r12, rax

	dec rcx
	jnz LOOP_A
	
	mov rcx, r10
	jz END_PROC

LOOP_B:
	
	xor rax, rax
	xor rbx, rbx

	mov al, byte ptr [r13 - 1]
	mov bl, byte ptr [r13]
	add rax, rbx
	mov bl, byte ptr [r13 + 1]
	add rax, rbx

	mov bl, byte ptr [r11 - 1]
	add rax, rbx
	mov bl, byte ptr [r11]
	add rax, rbx
	mov bl, byte ptr [r11 + 1]
	add rax, rbx
	
	mov bl, byte ptr [r14 - 1]
	add rax, rbx
	mov bl, byte ptr [r14]
	add rax, rbx
	mov bl, byte ptr [r14 + 1]
	add rax, rbx
	
	div r15b

	mov byte ptr[r12], al

	inc r13
	inc r11
	inc r14
	inc r12 

	dec rcx
	jnz LOOP_B

END_PROC:

	movdqu xmm15, xmmword ptr [rsp]
	add rsp, 16
	movdqu xmm14, xmmword ptr [rsp]
	add rsp, 16
	movdqu xmm12, xmmword ptr [rsp]
	add rsp, 16
	movdqu xmm11, xmmword ptr [rsp]
	add rsp, 16
	movdqu xmm10, xmmword ptr [rsp]
	add rsp, 16
	movdqu xmm9, xmmword ptr [rsp]
	add rsp, 16
	movdqu xmm8, xmmword ptr [rsp]
	add rsp, 16
	movdqu xmm7, xmmword ptr [rsp]
	add rsp, 16
	movdqu xmm6, xmmword ptr [rsp]
	add rsp, 16
	movdqu xmm5, xmmword ptr [rsp]
	add rsp, 16


	pop r15
	pop r13
	pop r12
	pop r11
	pop r10

ret


LowPassFilterProc ENDP
ArrayMul	db 28, 28, 28, 28, 28, 28, 28, 28
ArrayDiv	db 9, 9, 9, 9, 9, 9, 9, 9 
END

