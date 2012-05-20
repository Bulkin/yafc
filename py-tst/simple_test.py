from twisted.internet.protocol import ClientFactory
from twisted.protocols.basic import LineReceiver
from twisted.internet import reactor
from twisted.internet import task
from time import time,sleep
import sys

# ugly but works
users = []
SEND_INTERVAL = 1

class ChatUser(LineReceiver):
    DEF_ROOM = 'all'
    counter = 0
    timeout = 5

    def connectionMade(self):
        self.name = 'tester' + str(ChatUser.counter)
        ChatUser.counter += 1
        self.response_times = []
        self.room = self.DEF_ROOM
        self.expected = ''
        self.time = 0
        self.errors = 0
        self.msg_counter = 0
        users.append(self)
        self.sendLine('test message from ' + self.name)
        reactor.callLater(0.1, self.set_name)

    def expect(self, msg):
        if self.expected != '':
            if time() - self.time > self.timeout:
                # ideally this should never happen on a good network
                self.expected = ''
                self.errors += 1
                print self.name + ' didn''t receive proper response'
            else:
                return False
        self.sendLine(msg)        
        self.expected = self.name + '->' + self.room + '> ' + msg;
        self.time = time()
        return True

    def lineReceived(self, line):
        if line.strip() == self.expected:
            self.expected = ''
            self.response_times.append(time() - self.time)

    def mean_response(self):
        if len(self.response_times) == 0:
            return 0
        return sum(self.response_times) / len(self.response_times)

    def total_errors(self):
        return self.errors

    def set_name(self):
        self.sendLine('%name ' + self.name) 
        reactor.callLater(SEND_INTERVAL, self.send_test_msg)       
    
    def send_test_msg(self):
        self.expect('test msg {}'.format(self.msg_counter))
        self.msg_counter += 1
        reactor.callLater(SEND_INTERVAL, self.send_test_msg)       
                    

def print_stats():
    if len(users) == 0:
        resp = 0
    else:
        resp = sum([ i.mean_response() for i in users ]) / len(users)
    err = sum([ i.total_errors() for i in users ])
    print '{:.3f} s, err = {}'.format(resp, err)
    
class ChatUserFactory(ClientFactory):
    protocol = ChatUser
    
    def clientConnectionFailed(self, connector, reason):
        print 'connection failed:', reason.getErrorMessage()
        reactor.stop()

    def clientConnectionLost(self, connector, reason):
        print 'connection lost:', reason.getErrorMessage()
        reactor.stop()   

def main(host, port, count = 200, send_interval = 1, print_interval = 3):
    SEND_INTERVAL = send_interval
    for i in range(count):
        factory = ChatUserFactory()
        reactor.connectTCP(host, port, factory)
    task.LoopingCall(print_stats).start(print_interval)    
    reactor.run()

def show_help():
    print 'simple_test host port'
    sys.exit(0)
    
if __name__ == '__main__':
    if len(sys.argv) < 3:
        show_help()
    host = sys.argv[1]
    port = int(sys.argv[2])
    if len(sys.argv) > 3:
        count = int(sys.argv[3])
    main(host, port, count)
