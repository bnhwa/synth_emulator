# -*- coding: utf-8 -*-
"""
Created on Thu Apr 22 12:49:28 2021

@author: bb339
"""
bpm = 8
class channel:
    def __init__(self,str_in, wav = 0,oct_shift = 0, shift = 0):
        self.notes = self.format_channel(str_in,wav = wav,oct_shift=oct_shift)
    def repvals(self,str_in):
        for i in [" ", "\t"]:
            str_in = str_in.replace(i,"")
        return str_in
    
        pass
    
    def adjust_notes(self,l,oct_shift = 0,scale = 0,shift = 0, shift_mes = True):
        global bpm
        l[2]=float(l[2])
        l[3]=float(l[3])
        if oct_shift:
            l[1]=str(int(l[1])+oct_shift)
        if shift:
            offset =  shift*bpm if shift_mes else shift
            l[3]+=offset#=float(l[3])+offset
        if scale:
            l[2]*=scale#float(l[2])*scale)
            l[3]*=scale#str(float(l[3])*scale)
        return l
            
    
        
    def format_channel(self,note_arr,wav = 0, oct_shift = 0,scale = 0):
        global bpm
        ret = []
        for idx,v in enumerate(note_arr):
            ret+=[self.adjust_notes(list(i.split(","))+[str(wav)],\
                        oct_shift = oct_shift,scale = scale, shift = idx,shift_mes = True)\
            for i in self.repvals(v).split("\n") if i]
        ret.sort(key = lambda x: x[3])
        return ret
    
    def printsched(self):
        for i in self.notes:
            print("apu.add_note_queue({});".format(",".join([str(k) for k in i])))
    def merge_channels(self,other):
        """
        merge multiple channels, order by position of note start
        """
        self.notes+=other.notes
        self.notes.sort(key = lambda x: x[3])
        
if __name__ == '__main__':
    pass
    #note,oct,length,pos,wavelength
    rh = ["""
    A,4,1.1,0
    GS,4,1.1,1
    E,4,1.1,2
    CS,4,1.1,3
    B,3,1.1,4
    CS,4,3,5
    """,
    """
    FS,4,1.1,0
    E,4,1.1,1
    D,4,1.1,2
    A,3,1.1,3
    G,3,1.1,4
    A,3,3,5
    """,
    """
    F,4,1.1,0
    E,4,1.1,1
    D,4,1.1,2
    AS,3,1.1,3
    C,4,1.1,4
    G,4,3,5
    """,
    """
    FS,4,1,0
    F,4,1,0.5
    E,4,7,1
    """
    ]
    rhc = channel(rh,oct_shift = 1)
    # rhc.printsched()
    
    lh = [
    """
    CS,3,7,1
    A,2,7,1
    """,
    """
    C,3,7,1
    A,2,7,1
    """,
    """
    D,3,4,1
    AS,2,7,1
    AS,2,3,5
    G,2,3,5
    """,
    """
    G,3,7,1
    E,3,7,1
    CS,3,7,5
    A,2,7,5
    """
    ]
    lhc = channel(lh,wav = 0,oct_shift = 1)
    # lhc = cha
    # lhc.printsched()


    # song_in = []
    # song_in.sort(key = lambda test_list: test_list[3])
    rhc.merge_channels(lhc)
    rhc.printsched()