class BrainfuckMachine:
    def __init__(self, tapeLength):
        self.code = ''
        self.tape = [0] * tapeLength
        self.head = 0


    def run(self):
        i = 0
        currentChar = ''

        while i < len(self.code):
            currentChar = self.code[i] 

            if currentChar == '+':
                self.tape[self.head] += 1
                if self.tape[self.head] == 256:
                    self.tape[self.head] = 0
            elif currentChar == '-': 
                 self.tape[self.head] -= 1
                 if self.tape[self.head] == -1:
                    self.tape[self.head] = 255
            elif currentChar == '>':
                self.head += 1
                if self.head > 64:
                    raise BrainfuckMachine.HeadOverflow
            elif currentChar == '<':
                self.head -= 1                    
                if self.head < 0:
                    raise BrainfuckMachine.HeadOverflow

            elif currentChar == '[':
                if self.tape[self.head] == 0:
                    i = self.matchOpenBracket(i)

            elif currentChar == ']':
                if self.tape[self.head] != 0:
                    i = self.matchClosedBracket(i)


            i += 1  


    def matchOpenBracket(self, openPos):
        closePos = openPos
        counter = 1

        while counter > 0:
            closePos += 1
            if closePos >= len(self.code):
                raise self.BracketMismatch           

            c = self.code[closePos]
            if c == '[':
                counter += 1
            elif c == ']':
                counter -= 1
        
        return closePos

    def matchClosedBracket(self, closePos):
        openPos = closePos
        counter = 1
                
        while counter > 0: 
            openPos -= 1           
            if openPos < 0:
                raise self.BracketMismatch            
            
            c = self.code[openPos]
            if c == '[':
                counter -= 1
            elif c == ']':
                counter += 1

        return openPos

    class HeadOverflow(Exception):
        pass  

    class BracketMismatch(Exception):
        pass    

