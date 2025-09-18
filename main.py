import sys
from PySide6 import QtWidgets

class PhotoEditor(QtWidgets.QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Photo Editor")
        self.setGeometry(100, 100, 800, 600)
        
        self.active_file = None
        self.active_file_path = None
        
        menu_bar = self.menuBar()
        self.file_menu_bar = menu_bar.addMenu("File")
        self.file_menu_bar.addAction("Open File").triggered.connect(self.open_file)
        self.file_menu_bar.addAction("Save File")
        self.file_menu_bar.addAction("About")
        self.file_menu_bar.addAction("Exit").triggered.connect(self.close)
        
        central_widget = QtWidgets.QWidget()
        
        central_widget.setLayout(QtWidgets.QBoxLayout(QtWidgets.QBoxLayout.Direction.LeftToRight))
        layout = central_widget.layout()
        layout.addWidget(QtWidgets.QLabel("Photo Editor Application"))
        layout.addWidget(QtWidgets.QLabel("Image will be displayed here"))
        
        self.setCentralWidget(central_widget)
    
    def open_file(self):
        file_dialog = QtWidgets.QFileDialog(self)
        file_dialog.show()
        file_dialog.setNameFilter("Images (*.png *.xpm *.jpg)")
        if file_dialog.exec():
            file_path = file_dialog.selectedFiles()[0]
            self.load_image(file_path)
            print(f"Opened file: {file_path}")
    
    def load_image(self, file_path):
        self.active_file_path = file_path
        
        
        pass
     
if __name__ == "__main__":
        app = QtWidgets.QApplication([])
        
        photo_editor = PhotoEditor()
        photo_editor.show()
        
        sys.exit(app.exec())    