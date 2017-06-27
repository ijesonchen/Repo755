from pbdef import *



pbline = r'''


package ULPB;

//////////////////////////////////////////////////////////////////////////
// common message
message RetCode
{
    required    int32   code = 1;    // 返回值。0：成功，非0：出现错误
    optional    string  msg = 2;    // 出现错误时的详细信息
}

//////////////////////////////////////////////////////////////////////////
// call process

// 11种: vad, front-end (vadseg), dtmf, lang, gender, spk, sseg, kwd, trans, de-ring, de-duplicate
enum AudioRecogFlag
{
    ARF_Unknown                         = 0; // 未定义
    ARF_Preprocess                      = 1;  // 有效音判定，语种，性别
    ARF_Speaker                         = 2;  // 说话人
    ARF_Keyword                         = 4;  // 关键词
    ARF_Transcript                      = 8;  // 全文
    ARF_VAD                             = 16; // 端点检测（有效音片段）
    ARF_SpeechSegment                   = 32; // 固定音频
    ARF_DTMF                            = 64; // 拨号音
    ARF_DeRing                          = 512;  // 振铃
    ARF_RtTrans                         = 1024; // 实时转录
}

enum GenderType
{
    GT_Male     = 1; // 男
    GT_Female   = 2; // 女
    GT_Both     = 3; // 男和女
}



'''

class LineType(Enum):
    space = 0
    comment = 1
    msg = 2
    enum = 3
    body = 4
    beg = 5
    end = 6


def GetLineType(line):
    s = line.strip()
    if len(s) == 0:
        return LineType.space, ''
    n = '{'
    if s[:len(n)] == n:
        return LineType.beg, ''
    n = '}'
    if s[:len(n)] == n:
        return LineType.end, ''
    n = 'package'
    if s[:len(n)] == n:
        return LineType.space, ''
    n = '//'
    if s[:len(n)] == n:
        t = s.replace('/', '')
        if len(t) == 0:
            return LineType.space, ''
        return LineType.comment, t
    n = 'message'
    if s[:len(n)] == n:
        return LineType.msg, s
    n = 'enum'
    if s[:len(n)] == n:
        return LineType.enum, s

    return LineType.body, s

def ParseMsg(line):
    return

def ParseEnum(line):
    return

def ParseLine(line, isMsg):
    if isMsg:
        return ParseMsg(line)
    else:
        return PraseEnum(line)


if __name__ == '__main__':
    lines = pbline.split('\n')
    defs = []
    inbody = false
    inm = false
    msg = Pbdef()
    ebody = PbEbody()
    mbody = PbMbody()
    for line in lines:
        linetype, str = GetLineType(line)
        if linetype == LineType.space:
            continue
        elif linetype == LineType.comment:
            p = 0
            if inbody:
                p = msg
            elif inm:
                p = mbody
            else:
                p = ebody            
            p.comment = ('%s %s' % (p.comment, str) )
        elif linetype == LineType.beg:
            inbody = True
        elif linetype == LineType.end:
            inbody = False
        elif linetype == LineType.msg:
            mbody = True
        elif linetype == LineType.enum:
            mbody = False
        else:
            pass


    

