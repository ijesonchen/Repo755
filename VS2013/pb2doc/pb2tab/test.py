class Node(object):
    def __init__(self, val, ptr = 0):
        self.data = val
        self.next = ptr

    def show(self):
        p = self
        while(p):
            print(p.data)
            p = p.next

x = Node(23)
x.next = Node(46)
x.next.next = Node(88)

x.show()