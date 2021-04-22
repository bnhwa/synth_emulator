# -*- coding: utf-8 -*-
"""
Created on Thu Apr 22 12:49:28 2021

@author: bb339
"""
bpm = 8
def repvals(str_in):
    for i in [" ", "\t"]:
        str_in = str_in.replace(i,"")
    return str_in
def oct_shift(l,o):
    l[1]=str(int(l[1])+o)
    return l

def scaleNotes(l,scale):
    l[2]=str(float(l[2])*scale)
    l[3]=str(float(l[3])*scale)
    pass
def format_notes(str_in, wav = 0, o_shift = 0,scale = 0):
    global bpm
    if scale:
        bpm*=scale
        return [scaleNotes(oct_shift(list(i.split(",")))+[str(wav)],scale) for i in repvals(str_in).split("\n") if i]
    else:
        return [oct_shift(list(i.split(",")),o_shift)+[str(wav)] for i in repvals(str_in).split("\n") if i]
        
    
def adjust_notes(l,o_shift = 0,scale = 0,shift = 0, shift_mes = True):
    global bpm
    l[2]=float(l[2])
    l[3]=float(l[3])
    if o_shift:
        l[1]=str(int(l[1])+o_shift)
    if shift:
        offset =  shift*bpm if shift_mes else shift
        l[3]+=offset#=float(l[3])+offset
    if scale:
        l[2]*=scale#float(l[2])*scale)
        l[3]*=scale#str(float(l[3])*scale)
    return l
        

    
def format_channel(note_arr,wav = 0, o_shift = 0,scale = 0):
    global bpm
    ret = []
    for idx,v in enumerate(note_arr):
        ret+=[adjust_notes(list(i.split(","))+[str(wav)],\
                    o_shift = o_shift,scale = scale, shift = idx,shift_mes = True)\
        for i in repvals(v).split("\n") if i]
    ret.sort(key = lambda x: x[3])
    return ret
def addMeas(song,meas):
    return song
def printsched(l):
    for i in l:
        print("apu.add_note_queue({});".format(",".join([str(k) for k in i])))

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
    FS,4,0.5,0
    F,4,0.5,0.5
    E,4,7,1
    """
    ]
    rh = format_channel(rh)
    printsched(rh)


    # song_in = []
    # song_in.sort(key = lambda test_list: test_list[3])