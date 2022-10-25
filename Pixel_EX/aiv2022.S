.org $8000
start:
LDX #$FF
TXS 

; Default pixel value
LDA #1
LDX #$78 
STA $0200, X

loop:
    LDA $4000 ; Loads in A the input value

    CMP #08
    BEQ goRight
    CMP #04
    BEQ goLeft
    CMP #01
    BEQ goUp
    CMP #02
    BEQ goDown

    CMP #05
    BEQ goUpLeft
    CMP #09
    BEQ goUpRight
    CMP #06
    BEQ goDwLeft
    CMP #$0A
    BEQ goDwRight

    BNE loop ; keeps looping


goRight:
    ; Resets current pixel
    LDA #0 
    STA $0200, X ; STA $0200, X ; 0x0200 + X | Store Accumulator in Memory
    INX
    ; Prints next pixel
    LDA #1
    STA $0200, X
    ; Goes back looping
    JMP loop


goLeft:

    LDA #0 
    STA $0200, X ; STA $0200, X ; 0x0200 + X | Store Accumulator in Memory
    DEX

    LDA #1
    STA $0200, X

    JMP loop

goUp:

    LDA #0 
    STA $0200, X ; STA $0200, X ; 0x0200 + X | Store Accumulator in Memory
    
    TXA
    SBC #$10  
    TAX

    LDA #1
    STA $0200, X

    JMP loop

goDown:

    LDA #0 
    STA $0200, X ; STA $0200, X ; 0x0200 + X | Store Accumulator in Memory
    
    TXA
    ADC #$0f  
    TAX

    LDA #1
    STA $0200, X

    JMP loop

goUpLeft:

    LDA #0 
    STA $0200, X ; STA $0200, X ; 0x0200 + X | Store Accumulator in Memory
    
    TXA
    SBC #$11  
    TAX

    LDA #1
    STA $0200, X

    JMP loop

goUpRight:

    LDA #0 
    STA $0200, X ; STA $0200, X ; 0x0200 + X | Store Accumulator in Memory
    
    TXA
    SBC #$0f  
    TAX

    LDA #1
    STA $0200, X

    JMP loop

goDwLeft:

    LDA #0 
    STA $0200, X ; STA $0200, X ; 0x0200 + X | Store Accumulator in Memory
    
    TXA
    ADC #$0E 
    TAX

    LDA #1
    STA $0200, X

    JMP loop

goDwRight:

    LDA #0 
    STA $0200, X ; STA $0200, X ; 0x0200 + X | Store Accumulator in Memory
    
    TXA
    ADC #$10  
    TAX

    LDA #1
    STA $0200, X

    JMP loop

.goto $FFFA
.dw start ; non maskable
.dw start ; reset
.dw start ; interrupt
