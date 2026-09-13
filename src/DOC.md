The Game Boy’s custom CPU (the Sharp LR35902) is a hybrid. It uses an Intel 8080 as its base architecture but adopts key enhancements and extended instructions from the Zilog Z80—most notably the CB-prefixed instructions.Specific instructions and features borrowed from the Z80 include:Bit Manipulation: The Z80 brought in the crucial 0xCB prefix extender. This enabled the Game Boy to execute bit test, set, and reset operations directly, saving massive amounts of processing overhead.Shift and Rotate: It includes Z80-style operations for bitwise shifting and rotating on registers other than just the Accumulator.Relative Jump: It uses Z80's JR (Jump Relative) instructions, which were absent on the original Intel 8080.Z80-style Syntax: Game Boy development uses standard Zilog mnemonics for assembly code.While the Game Boy borrowed these Z80 features, it also stripped out many legacy 8080/Z80 capabilities. It completely lacks the Z80's block move/I/O instructions (ED prefix), the secondary register set, and index registers (IX, IY).

# FUNDAMENTS OF GAMEBOY
https://gbdev.io/pandocs/Rendering.html

# GAMEBOY ARCHITECTURE
https://www.copetti.org/writings/consoles/game-boy/