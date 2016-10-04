import re

import msgpack
import nanomsg
from PyQt5.QtCore import QDateTime, Qt, QThread
from PyQt5.QtGui import QColor
from PyQt5.QtWidgets import QFrame, QStyle, QTreeWidgetItem
from nanomsg import Socket, SUB, SUB_SUBSCRIBE

from playground.ui.logwidget import Ui_LogWidget

RESOURCE_NAME_RE = re.compile("^\[([^\]]+)\]")

LOG_COLOR = {
    'info': QColor("blue"),
    'warning': QColor("yellow"),
    'debug': QColor("green"),
    'error': QColor("red"),
}

LOG_ICON = {
    'info': QStyle.SP_MessageBoxInformation,
    'warning': QStyle.SP_MessageBoxWarning,
    'debug': QStyle.SP_MessageBoxQuestion,
    'error': QStyle.SP_MessageBoxCritical,
}


class LogSub(QThread):
    def __init__(self, url):
        self.socket = Socket(SUB)
        self.url = url
        self.handlers = []

        super().__init__()

    def register_handler(self, handler):
        self.handlers.append(handler)

    def run(self):
        self.socket.set_string_option(SUB, SUB_SUBSCRIBE, b'')
        self.socket.connect(self.url)
        while True:
            try:
                msg = self.socket.recv()
                msg_pack = msgpack.unpackb(msg, encoding='utf-8')

                for h in self.handlers:
                    h(**msg_pack)

            except nanomsg.NanoMsgAPIError as e:
                raise


class LogWidget(QFrame, Ui_LogWidget):
    def __init__(self, ignore_where=None):
        super(LogWidget, self).__init__()
        self.setupUi(self)

        self.instance = None
        self.ignore_where = None
        if ignore_where is not None:
            self.set_ignore_where(ignore_where)

        self.log_tree_widget.header().setStretchLastSection(True)

    def set_instance(self, instance):
        url = instance.log_url if isinstance(instance.log_url, bytes) else instance.log_url.encode()
        self.instance = instance
        self.logsub = LogSub(url)
        self.logsub.register_handler(self.add_log)
        self.logsub.start(QThread.NormalPriority)

    def set_ignore_where(self, pattern):
        self.ignore_where = re.compile(pattern)

    def _is_ignored(self, name):
        if not self.ignore_where:
            return False

        m = self.ignore_where.match(name)
        return m is not None

    def add_log(self, time, level, worker_id, where, msg):
        # print(time, level, worker, where, msg)
        if self._is_ignored(where):
            return

        dt_s = QDateTime.fromTime_t(time).toString("hh:mm:ss.zzz")

        item = QTreeWidgetItem(['', dt_s, str(worker_id), where, msg[:-1]])

        item.setIcon(0, self.style().standardIcon(LOG_ICON[level]))
        item.setData(0, Qt.UserRole, level)

        self.log_tree_widget.addTopLevelItem(item)

        sb = self.log_tree_widget.verticalScrollBar()
        sb.setValue(sb.maximum())

    def item_double_clicked(self, item, idx):
        """

        :type item: QTreeWidgetItem
        """

        level = item.data(0, Qt.UserRole)
        if level != 'error':
            return

        where = item.text(3)
        if 'resource_compiler' not in where:
            return

        msg = item.text(4)

        resource_name = RESOURCE_NAME_RE.match(msg).group(1)
        # self.script_editor.open_resource(resource_name)
