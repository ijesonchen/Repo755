class PbEbody(object):
    __slots__ = ('name', 'value', 'comment')
        
    def __init__(self, name = '', value = 0, comment = ''):
        self.name = name
        self.value = value
        self.comment = comment

    def __str__(self):
        title = ''
        if len(self.comment):
            title = ('// %s\n  ' % self.comment)
        title = ('%s%s %s'%(title, self.name, self.value))
        return title

class PbMbody(object):
    __slots__ = ('rule', 'type', 'name', 'tag', 'default', 'comment')

    def __init__(self, rule = '', type = '', name = '', tag = 0, comment = '', default = ''):
        self.rule = rule
        self.type = type
        self.name = name
        self.tag = tag
        self.default = default
        self.comment = comment

    def __str__(s):
        l = ''
        if len(s.comment):
            l = ('// %s\n  ' % s.comment)
        l = ('%s%s %s %s = %d' % (l, s.rule, s.type, s.name, s.tag) )
        tmp = str(s.default)
        if len(tmp):
            l = ('%s [%s]' % (l, tmp) )
        return l


class Pbdef(object):
    __slots__ = ('type', 'name', 'comment', 'bodys')
        
    def __init__(self, type = '', name = '', bodys = [], comment = ''):
        self.type = type
        self.name = name
        self.comment = comment
        self.bodys = bodys

    def __str__(self):
        title = ''
        if len(self.comment):
            title = ('// %s\n' % self.comment)
        title = ('%s%s %s:\n' % (title, self.type, self.name))
        body = ''
        for b in self.bodys:
            t = ('  %s\n'%b)
            body  = body + t
        return title + body

if __name__ == '__main__':
    d = Pbdef()

    d.type = 'message'
    d.name = 'callreq'
    d.comment = 'call process request'

    d.bodys.append(PbMbody('optional', 'string', 'id', 1, 'call id', 1))
    d.bodys.append(PbMbody('optional', 'string', 'path', 2))

    l = []
    l.append(PbEbody('mon', 1, 'first'))
    l.append(PbEbody('tues', 2))

    d2 = Pbdef('enum', 'day', l)


    print(d)
    print(d2)
