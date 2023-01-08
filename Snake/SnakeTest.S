.org $8000

.define seed $98
.define RandomGenerated $9A
.define SnakeLength $9B
.define ApplePosition $9C

LDA #0
STA RandomGenerated
STA seed 
;inc SnakeLength
JSR drawSnake

;--------------------------------
start:
;--------------------------------
JSR moveSnake

JSR drawApple
JMP start

;--------------------------------
moveSnake:
;--------------------------------
    
    LDA $4000 ; read from the keyboard
    CMP #$01
    BEQ up
    LDA $4000 ; read from the keyboard
    CMP #$02
    BEQ down
    LDA $4000 ; read from the keyboard
    CMP #$04
    BEQ left
    LDA $4000 ; read from the keyboard
    CMP #$08
    BEQ right

     

    LDA $4000 ; read from the keyboard
    CMP #$71  ;"Q"

RTS



;BEQ gameOver ;TODO: Sistemare il "Quit"
;RTS

up:
    
    TYA         ;Metto la posizione del serpente da Y in A
    ADC #$eF    ;Mi sposto di una riga in su

    STA $FF     ;APPOGGIO A IN FF
    TAX         ;Carico A in X 

    LDA $0200, X    ;Carico in A il valore disegnato 

    CMP #09         ;Se c'è una mela in quella posizione
    BNE NotEqual1  ;Non pulisco la posizione del serpente
Equal1:
    JSR AteApple
    JSR drawSnake
    JSR Random 
    ;TAY
    RTS
NotEqual1:

    JSR clearsnake
    LDY $FF
    JSR drawSnake
    RTS

down:
    
    TYA
    ADC #$0F

    STA $FF 
    TAX

    LDA $0200, X 

    CMP #09
    BNE NotEqual2
Equal2:
    JSR AteApple
    JSR drawSnake
    JSR Random 
    ;TAY
    RTS
NotEqual2:
    JSR clearsnake
    LDY $FF
    JSR drawSnake
    RTS 

left:
    TYA
    STA $FF     ;APPOGGIO A IN FF
    TAX         ;Carico A in X 
    DEX

    LDA $0200, X    ;Carico in A il valore disegnato 

    CMP #09         ;Se c'è una mela in quella posizione
    BNE NotEqual3
Equal3:
    JSR AteApple
    DEY
    JSR drawSnake
    JSR Random 
    RTS
NotEqual3:
    JSR clearsnake
    LDY $FF
    DEY
    JSR drawSnake
    RTS

right:
    TYA
    STA $FF     ;APPOGGIO A IN FF
    TAX         ;Carico A in X 
    INX

    LDA $0200, X    ;Carico in A il valore disegnato 


    CMP #09         ;Se c'è una mela in quella posizione
    BNE NotEqual4
Equal4:
    JSR AteApple
    INY
    JSR drawSnake
    JSR Random 
    RTS
NotEqual4:
    JSR clearsnake
    LDY $FF
    INY
    JSR drawSnake
    RTS

AteApple:
    inc SnakeLength ;9B

    LDY $FF

    TYA
    LDX SnakeLength
    STA $0300, X

    LDX #0
    STX $9A ;mette flag a 0 per poter aggiornare il random

    LDA #0
    RTS
;--------------------------------
clearsnake:
;--------------------------------
LDA #0
STA $0200,Y
RTS


;--------------------------------
drawSnake:
;--------------------------------

CLC
;LoopTail
LDA #01
CMP SnakeLength
BEQ BranchTrue

BranchFalse:
    ;Finché non aggiorno in $0300 c'è la vecchia posizione
    STY $0300 ;Aggiorno con la nuova posizione della testa del serpente

    STY $99

    LDA #05
    STA $0200, Y

    TYA
    RTS

BranchTrue:
    JSR drawTail
    JMP BranchFalse

drawTail:
    LDA SnakeLength
    ;LDX $0300, A ;$0301

    ;Cancella l'ultima posizione di 01
    LDA #00      
    STA $0200, X
   
    LDX $0300 ;Carica la posizione di 0 in X 
    STX $0301 ;La Scrive in 01

    LDA #05
    STA $0200, X
    RTS

;--------------------------------
drawApple:
;--------------------------------
STY $99
LDA RandomGenerated
CMP #0
BEQ Random 
LDY $99 ;Rimetto in A la posizione del serpente 

RTS

Random:
    lda seed ;Carico il seed con la posizione della testa del serpente
    beq doEor
    asl A
    beq noEor ;if the input was $80, skip the EOR
    bcc noEor
doEor:    
    eor #$1d
noEor:  
    sta seed
    TAY
    LDA #09
    STA $0200,Y

    LDA #01
    STA RandomGenerated
    
    LDY $99 ;Rimetto in A la posizione del serpente 
    
    RTS

;--------------------------------
gameOver:
;--------------------------------

.goto $FFFA
.dw start
.dw start
.dw start
