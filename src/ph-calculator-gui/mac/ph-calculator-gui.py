"""
version : v3.1.0-alpha

MIT License

Copyright (c) 2019-2020 Lee Kyung-ha <i_am@nulleekh.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""

import sys
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5 import uic
from ctypes import *
# Import modules.

MAX_NUMBER_LENGTH = 255
NUMBER_OF_DB_SOLUTE = 255
NUMBER_OF_DB_DATA = 6
LENGTH_OF_DB_DATA = 32
MAX_SOLUTION_NUMBER = 255
PH_CALCULATOR_STARTPOINT = c_double(-3)
PH_CALCULATOR_ENDPOINT = c_double(18)
PH_CALCULATOR_RESOLUTION = c_double(0.000001)
PH_CALCULATOR_INITIALINTERVAL = c_double(1)
# Declare variables(as constants) for operating.

main_window_ui = uic.loadUiType("gui/general/main_window.ui")[0]
result_window_ui = uic.loadUiType("gui/general/result_window.ui")[0]
about_ui = uic.loadUiType("gui/general/about.ui")[0]
strong_solution_ui = uic.loadUiType("gui/general/strong_solution.ui")[0]
type_1_ui = uic.loadUiType("gui/general/type_1.ui")[0]
type_2_ui = uic.loadUiType("gui/general/type_2.ui")[0]
# Load ui files which generally used.

ph_calculator_ui = uic.loadUiType("gui/ph-calculator/ph_calculator.ui")[0]
# Load ui file for ph-calculator.

select_titrant_ui = uic.loadUiType("gui/general/select_titrant.ui")[0]
# Load ui file for graph-generator and recipe-finder.

graph_generator_ui = uic.loadUiType("gui/graph-generator/graph_generator.ui")[0]
strong_titrant_graph_ui = uic.loadUiType("gui/graph-generator/strong_titrant_graph.ui")[0]
type_1_titrant_graph_ui = uic.loadUiType("gui/graph-generator/type_1_titrant_graph.ui")[0]
type_2_titrant_graph_ui = uic.loadUiType("gui/graph-generator/type_2_titrant_graph.ui")[0]
# Load ui files for graph-generator.

recipe_finder_ui = uic.loadUiType("gui/recipe-finder/recipe_finder.ui")[0]
target_ph_recipe_ui = uic.loadUiType("gui/recipe-finder/target_ph_recipe.ui")[0]
strong_titrant_recipe_ui = uic.loadUiType("gui/recipe-finder/strong_titrant_recipe.ui")[0]
type_1_titrant_recipe_ui = uic.loadUiType("gui/recipe-finder/type_1_titrant_recipe.ui")[0]
type_2_titrant_recipe_ui = uic.loadUiType("gui/recipe-finder/type_2_titrant_recipe.ui")[0]
# Load ui files for recipe-finder.

pHCalcFunc = cdll.LoadLibrary('pHCalcFunc.dll')
# Load library file for operating.

input_title = "Default"


class MainWindow(QMainWindow, main_window_ui):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        # Setup ui.
        self.move(QDesktopWidget().availableGeometry().center() - self.frameGeometry().center())
        # Move window to center.
        self.calculatorButton.clicked.connect(self.calculator_clicked)
        self.generatorButton.clicked.connect(self.generator_clicked)
        self.finderButton.clicked.connect(self.finder_clicked)
        self.aboutButton.clicked.connect(self.about_clicked)

    @staticmethod
    def calculator_clicked():
        fw = Calculator()
        fw.exec_()

    @staticmethod
    def generator_clicked():
        fw = Generator()
        fw.exec_()

    @staticmethod
    def finder_clicked():
        fw = Finder()
        fw.exec_()

    @staticmethod
    def about_clicked():
        fw = About()
        fw.exec_()


class About(QDialog, about_ui):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        # Setup ui.
        self.move(QDesktopWidget().availableGeometry().center() - self.frameGeometry().center())
        # Move window to center.
        self.doneButton.clicked.connect(self.done_clicked)
        # Call done_clicked function when done button clicked.

    def done_clicked(self):
        self.close()
        # Close window.


class Calculator(QDialog, ph_calculator_ui):
    solution_type = "Default"

    def __init__(self):
        super().__init__()
        self.setupUi(self)
        # Setup ui.
        self.move(QDesktopWidget().availableGeometry().center() - self.frameGeometry().center())
        # Move window to center.
        self.enterButton.clicked.connect(self.enter_clicked)
        # Call enter_clicked function when enter button clicked.

    def enter_clicked(self):
        index_type_1 = (c_long * MAX_SOLUTION_NUMBER)(*range(MAX_SOLUTION_NUMBER))
        index_type_2 = (c_long * MAX_SOLUTION_NUMBER)(*range(MAX_SOLUTION_NUMBER))
        concentration_acid = (c_double * MAX_SOLUTION_NUMBER)(*range(MAX_SOLUTION_NUMBER))
        concentration_base = (c_double * MAX_SOLUTION_NUMBER)(*range(MAX_SOLUTION_NUMBER))
        concentration_type_1 = (c_double * MAX_SOLUTION_NUMBER)(*range(MAX_SOLUTION_NUMBER))
        mol_type_2 = (c_double * MAX_SOLUTION_NUMBER)(*range(MAX_SOLUTION_NUMBER))
        # Declare variables to collect input values.

        num_acid = int(self.strongAcid.text())
        if 0 > num_acid | num_acid > 255:
            print("ERROR - Invalid num_acid number.")
            print("INFO - Calculator ended.")
            exit()
        num_base = int(self.strongBase.text())
        if 0 > num_base | num_base > 255:
            print("ERROR - Invalid num_base number.")
            print("INFO - Calculator ended.")
            exit()
        num_type_1 = int(self.type1.text())
        if 0 > num_type_1 | num_type_1 > 255:
            print("ERROR - Invalid num_type_1 number.")
            print("INFO - Calculator ended.")
            exit()
        num_type_2 = int(self.type2.text())
        if 0 > num_type_2 | num_type_2 > 255:
            print("ERROR - Invalid num_type_2 number.")
            print("INFO - Calculator ended.")
            exit()
        # Get numbers of each elements.

        volume_all = float(self.pureWater.text())
        # Get volume of pure water.

        for i in range(num_acid):
            input_window = StrongSolution()
            input_window.strong_acid()
            input_window.exec_()

            volume_temp = float(input_window.volume.text())
            volume_all += volume_temp

            concentration_temp = float(input_window.concentration.text())

            concentration_acid[i] = (c_double(concentration_temp * volume_temp))
        # Get creation and volume of strong acid.

        for i in range(num_base):
            input_window = StrongSolution()
            input_window.strong_base()
            input_window.exec_()

            volume_temp = float(input_window.volume.text())
            volume_all += volume_temp

            concentration_temp = float(input_window.concentration.text())

            concentration_base[i] = (c_double(concentration_temp * volume_temp))
        # Get creation and volume of strong base.

        for i in range(num_type_1):
            input_window = Type1()
            input_window.exec_()

            name_temp = str(input_window.name.text())
            name_length = c_long(len(name_temp))

            index_type_1[i] = c_long(pHCalcFunc.SpecifyType1Solute(name_temp.encode(), name_length))

            volume_temp = float(input_window.volume.text())
            volume_all += volume_temp

            concentration_temp = float(input_window.concentration.text())

            concentration_type_1[i] = (c_double(concentration_temp * volume_temp))
        # Get creation and volume of type 1.

        for i in range(num_type_2):
            input_window = Type2()
            input_window.exec_()

            name_temp = str(input_window.name.text())
            name_length = c_long(len(name_temp))

            index_type_2[i] = c_long(pHCalcFunc.SpecifyType2Solute(name_temp.encode(), name_length))

            mol_type_2[i] = (c_double(float(input_window.mol.text())))
        # Get creation and mol of type 2.

        pHCalcFunc.PhCalculator.restype = c_double
        result = float(pHCalcFunc.PhCalculator(c_long(num_acid),
                                               c_long(num_base), c_long(num_type_1),
                                               c_long(num_type_2), concentration_acid, concentration_base,
                                               concentration_type_1,
                                               mol_type_2, index_type_1, index_type_2, c_double(volume_all)))
        # Call ph-calculator function to operate calculation.

        result_window = Result()
        result_window.represent_result(str("pH : " + str(round(result, 3))))
        result_window.exec_()
        # Print out "pH : " and result value on result window

        self.close()
        # Close window.


class Generator(QDialog, graph_generator_ui):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        # Setup ui.
        self.move(QDesktopWidget().availableGeometry().center() - self.frameGeometry().center())
        # Move window to center.
        self.enterButton.clicked.connect(self.enter_clicked)
        # Call enter_clicked function when enter button clicked.

    def enter_clicked(self):
        index_type_1 = (c_long * MAX_SOLUTION_NUMBER)(*range(MAX_SOLUTION_NUMBER))
        index_type_2 = (c_long * MAX_SOLUTION_NUMBER)(*range(MAX_SOLUTION_NUMBER))
        concentration_acid = (c_double * MAX_SOLUTION_NUMBER)(*range(MAX_SOLUTION_NUMBER))
        concentration_base = (c_double * MAX_SOLUTION_NUMBER)(*range(MAX_SOLUTION_NUMBER))
        concentration_type_1 = (c_double * MAX_SOLUTION_NUMBER)(*range(MAX_SOLUTION_NUMBER))
        mol_type_2 = (c_double * MAX_SOLUTION_NUMBER)(*range(MAX_SOLUTION_NUMBER))
        # Declare variables to collect input values.

        num_acid = int(self.strongAcid.text())
        if 0 > num_acid | num_acid > 255:
            print("ERROR - Invalid num_acid number.")
            print("INFO - Calculator ended.")
            exit()
        num_base = int(self.strongBase.text())
        if 0 > num_base | num_base > 255:
            print("ERROR - Invalid num_base number.")
            print("INFO - Calculator ended.")
            exit()
        num_type_1 = int(self.type1.text())
        if 0 > num_type_1 | num_type_1 > 255:
            print("ERROR - Invalid num_type_1 number.")
            print("INFO - Calculator ended.")
            exit()
        num_type_2 = int(self.type2.text())
        if 0 > num_type_2 | num_type_2 > 255:
            print("ERROR - Invalid num_type_2 number.")
            print("INFO - Calculator ended.")
            exit()
        # Get numbers of each elements.

        volume_all = float(self.pureWater.text())
        # Get volume of pure water.

        for i in range(num_acid):
            input_window = StrongSolution()
            input_window.strong_acid()
            input_window.exec_()

            volume_temp = float(input_window.volume.text())
            volume_all += volume_temp

            concentration_temp = float(input_window.concentration.text())

            concentration_acid[i] = (c_double(concentration_temp * volume_temp))
        # Get creation and volume of strong acid.

        for i in range(num_base):
            input_window = StrongSolution()
            input_window.strong_base()
            input_window.exec_()

            volume_temp = float(input_window.volume.text())
            volume_all += volume_temp

            concentration_temp = float(input_window.concentration.text())

            concentration_base[i] = (c_double(concentration_temp * volume_temp))
        # Get creation and volume of strong base.

        for i in range(num_type_1):
            input_window = Type1()
            input_window.exec_()

            name_temp = str(input_window.name.text())
            name_length = c_long(len(name_temp))

            index_type_1[i] = c_long(pHCalcFunc.SpecifyType1Solute(name_temp.encode(), name_length))

            volume_temp = float(input_window.volume.text())
            volume_all += volume_temp

            concentration_temp = float(input_window.concentration.text())

            concentration_type_1[i] = (c_double(concentration_temp * volume_temp))
        # Get creation and volume of type 1.

        for i in range(num_type_2):
            input_window = Type2()
            input_window.exec_()

            name_temp = str(input_window.name.text())
            name_length = c_long(len(name_temp))

            index_type_2[i] = c_long(pHCalcFunc.SpecifyType2Solute(name_temp.encode(), name_length))

            mol_type_2[i] = (c_double(float(input_window.mol.text())))
        # Get creation and mol of type 2.

        input_window = SelectTitrant()
        input_window.exec_()
        # Select type of titrant.

        time_titrant = -1
        volume_titrant = -1
        concentration_titrant = -1
        index_titrant = -1
        # Initialize  the value of variables that need for operation.
        if titrant_flag == 0:
            input_window = StrongTitrantGraph()
            input_window.strong_acid()
            input_window.exec_()

            time_titrant = int(input_window.times.text())
            volume_titrant = float(input_window.volume.text())
            concentration_titrant = float(input_window.concentration.text())
        # Get strong acid titrant's data.

        elif titrant_flag == 1:
            input_window = StrongTitrantGraph()
            input_window.strong_base()
            input_window.exec_()

            time_titrant = int(input_window.times.text())
            volume_titrant = float(input_window.volume.text())
            concentration_titrant = float(input_window.concentration.text())
        # Get strong base titrant's data.

        elif titrant_flag == 2:
            input_window = Type1TitrantGraph()
            input_window.exec_()

            name_temp = str(input_window.name.text())
            name_length = c_long(len(name_temp))

            index_titrant = int(pHCalcFunc.SpecifyType1Solute(name_temp.encode(), name_length))

            time_titrant = int(input_window.times.text())
            volume_titrant = float(input_window.volume.text())
            concentration_titrant = float(input_window.concentration.text())
        # Get type 1 titrant's data.

        elif titrant_flag == 3:
            input_window = Type2TitrantGraph()
            input_window.exec_()

            name_temp = str(input_window.name.text())
            name_length = c_long(len(name_temp))

            index_titrant = int(pHCalcFunc.SpecifyType2Solute(name_temp.encode(), name_length))

            time_titrant = int(input_window.times.text())
            concentration_titrant = float(input_window.mol.text())
        # Get type 2 titrant's data.

        pHCalcFunc.GraphGenerator.restype = c_int
        result = int(pHCalcFunc.GraphGenerator(c_long(num_acid),
                                               c_long(num_base), c_long(num_type_1),
                                               c_long(num_type_2), concentration_acid, concentration_base,
                                               concentration_type_1,
                                               mol_type_2, index_type_1, index_type_2, c_double(volume_all),
                                               c_long(titrant_flag), c_long(time_titrant), c_double(volume_titrant),
                                               c_double(concentration_titrant), c_long(index_titrant)))
        # Call graph-generator function to operate calculation.

        if result == 0:
            result_window = Result()
            result_window.represent_result("Graph Generated")
            result_window.exec_()
        # Print out "Graph Generated" on result window
        else:
            result_window = Result()
            result_window.represent_result("Operation Failed")
            result_window.exec_()
        # Print out "Operation Failed" on result window
        self.close()
        # Close window.


class Finder(QDialog, recipe_finder_ui):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        # Setup ui.
        self.move(QDesktopWidget().availableGeometry().center() - self.frameGeometry().center())
        # Move window to center.
        self.enterButton.clicked.connect(self.enter_clicked)
        # Call enter_clicked function when enter button clicked.

    def enter_clicked(self):
        index_type_1 = (c_long * MAX_SOLUTION_NUMBER)(*range(MAX_SOLUTION_NUMBER))
        index_type_2 = (c_long * MAX_SOLUTION_NUMBER)(*range(MAX_SOLUTION_NUMBER))
        concentration_acid = (c_double * MAX_SOLUTION_NUMBER)(*range(MAX_SOLUTION_NUMBER))
        concentration_base = (c_double * MAX_SOLUTION_NUMBER)(*range(MAX_SOLUTION_NUMBER))
        concentration_type_1 = (c_double * MAX_SOLUTION_NUMBER)(*range(MAX_SOLUTION_NUMBER))
        mol_type_2 = (c_double * MAX_SOLUTION_NUMBER)(*range(MAX_SOLUTION_NUMBER))
        # Declare variables to collect input values.

        num_acid = int(self.strongAcid.text())
        if 0 > num_acid | num_acid > 255:
            print("ERROR - Invalid num_acid number.")
            print("INFO - Calculator ended.")
            exit()
        num_base = int(self.strongBase.text())
        if 0 > num_base | num_base > 255:
            print("ERROR - Invalid num_base number.")
            print("INFO - Calculator ended.")
            exit()
        num_type_1 = int(self.type1.text())
        if 0 > num_type_1 | num_type_1 > 255:
            print("ERROR - Invalid num_type_1 number.")
            print("INFO - Calculator ended.")
            exit()
        num_type_2 = int(self.type2.text())
        if 0 > num_type_2 | num_type_2 > 255:
            print("ERROR - Invalid num_type_2 number.")
            print("INFO - Calculator ended.")
            exit()
        # Get numbers of each elements.

        volume_all = float(self.pureWater.text())
        # Get volume of pure water.

        for i in range(num_acid):
            input_window = StrongSolution()
            input_window.strong_acid()
            input_window.exec_()

            volume_temp = float(input_window.volume.text())
            volume_all += volume_temp

            concentration_temp = float(input_window.concentration.text())

            concentration_acid[i] = (c_double(concentration_temp * volume_temp))
        # Get creation and volume of strong acid.

        for i in range(num_base):
            input_window = StrongSolution()
            input_window.strong_base()
            input_window.exec_()

            volume_temp = float(input_window.volume.text())
            volume_all += volume_temp

            concentration_temp = float(input_window.concentration.text())

            concentration_base[i] = (c_double(concentration_temp * volume_temp))
        # Get creation and volume of strong base.

        for i in range(num_type_1):
            input_window = Type1()
            input_window.exec_()

            name_temp = str(input_window.name.text())
            name_length = c_long(len(name_temp))

            index_type_1[i] = c_long(pHCalcFunc.SpecifyType1Solute(name_temp.encode(), name_length))

            volume_temp = float(input_window.volume.text())
            volume_all += volume_temp

            concentration_temp = float(input_window.concentration.text())

            concentration_type_1[i] = (c_double(concentration_temp * volume_temp))
        # Get creation and volume of type 1.

        for i in range(num_type_2):
            input_window = Type2()
            input_window.exec_()

            name_temp = str(input_window.name.text())
            name_length = c_long(len(name_temp))

            index_type_2[i] = c_long(pHCalcFunc.SpecifyType2Solute(name_temp.encode(), name_length))

            mol_type_2[i] = (c_double(float(input_window.mol.text())))
        # Get creation and mol of type 2.

        input_window = TargetPH()
        input_window.exec_()
        # Get target pH value.

        input_window = SelectTitrant()
        input_window.exec_()
        # Select type of titrant.

        concentration_titrant = -1
        index_titrant = -1
        # Initialize  the value of variables that need for operation.

        if titrant_flag == 0:
            input_window = StrongTitrantRecipe()
            input_window.strong_acid()
            input_window.exec_()

            concentration_titrant = float(input_window.concentration.text())
        # Get strong acid titrant's data.

        elif titrant_flag == 1:
            input_window = StrongTitrantRecipe()
            input_window.strong_base()
            input_window.exec_()

            concentration_titrant = float(input_window.concentration.text())
        # Get strong base titrant's data.

        elif titrant_flag == 2:
            input_window = Type1TitrantRecipe()
            input_window.exec_()

            name_temp = str(input_window.name.text())
            name_length = c_long(len(name_temp))

            index_titrant = int(pHCalcFunc.SpecifyType1Solute(name_temp.encode(), name_length))

            concentration_titrant = float(input_window.concentration.text())
        # Get type 1 titrant's data.

        elif titrant_flag == 3:
            input_window = Type2TitrantRecipe()
            input_window.exec_()

            name_temp = str(input_window.name.text())
            name_length = c_long(len(name_temp))

            index_titrant = int(pHCalcFunc.SpecifyType2Solute(name_temp.encode(), name_length))
        # Get type 2 titrant's data.

        pHCalcFunc.RecipeFinder.restype = c_double
        result = float(pHCalcFunc.RecipeFinder(c_long(num_acid),
                                               c_long(num_base), c_long(num_type_1),
                                               c_long(num_type_2), concentration_acid, concentration_base,
                                               concentration_type_1,
                                               mol_type_2, index_type_1, index_type_2, c_double(volume_all),
                                               c_long(titrant_flag), c_double(concentration_titrant),
                                               c_long(index_titrant), c_double(target_ph)))
        # Call recipe-finder function to operate calculation.

        result_window = Result()
        result_window.represent_result("Result : " + str(round(result, 6)))
        result_window.exec_()
        # Print out "Result : " and result value on result window
        self.close()
        # Close window.


class StrongSolution(QDialog, strong_solution_ui):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        # Setup ui.
        self.move(QDesktopWidget().availableGeometry().center() - self.frameGeometry().center())
        # Move window to center.
        self.enterButton.clicked.connect(self.enter_clicked)
        # Call enter_clicked function when enter button clicked.

    def strong_acid(self):
        self.solutionType.setText("Strong Acid")
        # Print out "Strong Acid" in the window.

    def strong_base(self):
        self.solutionType.setText("Strong Base")
        # Print out "Strong Base" in the window.

    def enter_clicked(self):
        self.close()
        # Close window.


class Type1(QDialog, type_1_ui):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        # Setup ui.
        self.move(QDesktopWidget().availableGeometry().center() - self.frameGeometry().center())
        # Move window to center.
        self.enterButton.clicked.connect(self.enter_clicked)
        # Call enter_clicked function when enter button clicked.

    def enter_clicked(self):
        self.close()
        # Close window.


class Type2(QDialog, type_2_ui):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        # Setup ui.
        self.move(QDesktopWidget().availableGeometry().center() - self.frameGeometry().center())
        # Move window to center.
        self.enterButton.clicked.connect(self.enter_clicked)
        # Call enter_clicked function when enter button clicked.

    def enter_clicked(self):
        self.close()
        # Close window.


class SelectTitrant(QDialog, select_titrant_ui):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        # Setup ui.
        self.move(QDesktopWidget().availableGeometry().center() - self.frameGeometry().center())
        # Move window to center.
        self.strongAcidButton.clicked.connect(self.strong_acid_clicked)
        # Call strong_acid_clicked function when enter button clicked.
        self.strongBaseButton.clicked.connect(self.strong_base_clicked)
        # Call strong_base_clicked function when enter button clicked.
        self.type1Button.clicked.connect(self.type_1_clicked)
        # Call type_1_clicked function when enter button clicked.
        self.type2Button.clicked.connect(self.type_2_clicked)
        # Call type_2_clicked function when enter button clicked.

    def strong_acid_clicked(self):
        global titrant_flag
        titrant_flag = 0
        self.close()
        # Close window.

    def strong_base_clicked(self):
        global titrant_flag
        titrant_flag = 1
        self.close()
        # Close window.

    def type_1_clicked(self):
        global titrant_flag
        titrant_flag = 2
        self.close()
        # Close window.

    def type_2_clicked(self):
        global titrant_flag
        titrant_flag = 3
        self.close()
        # Close window.


class StrongTitrantGraph(QDialog, strong_titrant_graph_ui):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        # Setup ui.
        self.move(QDesktopWidget().availableGeometry().center() - self.frameGeometry().center())
        # Move window to center.
        self.enterButton.clicked.connect(self.enter_clicked)
        # Call enter_clicked function when enter button clicked.

    def strong_acid(self):
        self.solutionType.setText("Strong Acid")
        # Print out "Strong Acid" in the window.

    def strong_base(self):
        self.solutionType.setText("Strong Base")
        # Print out "Strong Base" in the window.

    def enter_clicked(self):
        self.close()
        # Close window.


class Type1TitrantGraph(QDialog, type_1_titrant_graph_ui):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        # Setup ui.
        self.move(QDesktopWidget().availableGeometry().center() - self.frameGeometry().center())
        # Move window to center.
        self.enterButton.clicked.connect(self.enter_clicked)
        # Call enter_clicked function when enter button clicked.

    def enter_clicked(self):
        self.close()
        # Close window.


class Type2TitrantGraph(QDialog, type_2_titrant_graph_ui):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        # Setup ui.
        self.move(QDesktopWidget().availableGeometry().center() - self.frameGeometry().center())
        # Move window to center.
        self.enterButton.clicked.connect(self.enter_clicked)
        # Call enter_clicked function when enter button clicked.

    def enter_clicked(self):
        self.close()
        # Close window.


class TargetPH(QDialog, target_ph_recipe_ui):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        # Setup ui.
        self.move(QDesktopWidget().availableGeometry().center() - self.frameGeometry().center())
        # Move window to center.
        self.enterButton.clicked.connect(self.enter_clicked)
        # Call enter_clicked function when enter button clicked.

    def enter_clicked(self):
        global target_ph
        target_ph = float(self.pH.text())
        self.close()
        # Close window.


class StrongTitrantRecipe(QDialog, strong_titrant_recipe_ui):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        # Setup ui.
        self.move(QDesktopWidget().availableGeometry().center() - self.frameGeometry().center())
        # Move window to center.
        self.enterButton.clicked.connect(self.enter_clicked)
        # Call enter_clicked function when enter button clicked.

    def strong_acid(self):
        self.solutionType.setText("Strong Acid")
        # Print out "Strong Acid" in the window.

    def strong_base(self):
        self.solutionType.setText("Strong Base")
        # Print out "Strong Base" in the window.

    def enter_clicked(self):
        self.close()
        # Close window.


class Type1TitrantRecipe(QDialog, type_1_titrant_recipe_ui):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        # Setup ui.
        self.move(QDesktopWidget().availableGeometry().center() - self.frameGeometry().center())
        # Move window to center.
        self.enterButton.clicked.connect(self.enter_clicked)
        # Call enter_clicked function when enter button clicked.

    def enter_clicked(self):
        self.close()
        # Close window.


class Type2TitrantRecipe(QDialog, type_2_titrant_recipe_ui):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        # Setup ui.
        self.move(QDesktopWidget().availableGeometry().center() - self.frameGeometry().center())
        # Move window to center.
        self.enterButton.clicked.connect(self.enter_clicked)
        # Call enter_clicked function when enter button clicked.

    def enter_clicked(self):
        self.close()
        # Close window.


class Result(QDialog, result_window_ui):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        # Setup ui.
        self.move(QDesktopWidget().availableGeometry().center() - self.frameGeometry().center())
        # Move window to center.
        self.doneButton.clicked.connect(self.done_clicked)
        # Call done_clicked function when done button clicked.

    def done_clicked(self):
        self.close()
        # Close window.

    def represent_result(self, result):
        self.result.setText(result)
        # Print out result data.


if __name__ == "__main__":
    application = QApplication(sys.argv)
    main_window = MainWindow()
    application.setWindowIcon(QIcon('gui/image/icon.png'))
    main_window.show()
    sys.exit(application.exec_())
