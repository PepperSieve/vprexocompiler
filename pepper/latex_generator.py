import numpy as np

from pylatex import Document, Section, Subsection, Tabular, Math, TikZ, Axis, \
    Plot, Figure, Matrix, Alignat, NoEscape
from pylatex.utils import italic
import matplotlib.pyplot as plt
import os

# Init
graph_name_list = ["Find Min", "Merging", "Binary Search", "KMP Search", "Next Permutation", "Dutch Flag", "Recurrence Relations Sequence", "Sum of Powers", "2D Convex Hull", "MSC"]
method_name_list = {"T_S": 0, "T_I": 1, "T_B": 2, "T_SB": 2, "T_BA": 3}
rec_file = open("result", "r")
line = rec_file.readline()
lineseg = line.split()

geometry_options = {"tmargin": "1cm", "lmargin": "10cm"}
doc = Document(geometry_options=geometry_options)

while lineseg[0] != "End":
    # Parser
    graph_name = graph_name_list[int(lineseg[1])]
    case_name = []
    #    T_S  T_I  T_B/T_SB  T_BA
    T = [ [],  [],       [],   []]

    line = rec_file.readline()
    lineseg = line.split()
    while not lineseg[0] in ["Benchmark", "End"]:
        method_name = method_name_list[lineseg[0]]
        
        line = rec_file.readline()
        lineseg = line.split()
        while not lineseg[0][:2] in ["T_", "Be", "En"]:
            tmp_name = ""
            # Don't want to parse LOG and EXP into names
            s = ""
            i = 0
            s = lineseg[0]
            while i < len(lineseg) - 1 and (len(s) < 3 or not s[:3] in ["LOG", "EXP"]):
                tmp_name += s + " "
                i += 1
                s = lineseg[i]
            tmp_name = tmp_name[:-2]

            if method_name == method_name_list["T_I"]:
                case_name.append(tmp_name)
                T[method_name].append(int(lineseg[-1]))
            elif tmp_name in case_name:
                T[method_name].append(int(lineseg[-1]))

            line = rec_file.readline()
            lineseg = line.split()
        while len(T[method_name]) != len(T[method_name_list["T_I"]]):
            T[method_name].append(0)

    # Grapher
    if __name__ == '__main__':


        with doc.create(Section('Example: ' + graph_name)):
            doc.append(italic('Data for some examples might not be finalized!!!'))

            with doc.create(Subsection('Table for ' + graph_name)):
                with doc.create(Tabular('l|llll')) as table:
                    table.add_row(("Input", "T_S", "T_I", "T_B", "T_B_Ann"))
                    table.add_hline()
                    for i in range(len(case_name)):
                        table.add_row((case_name[i], " " if not T[0] else "Timeout" if T[0][i] == 0 else T[0][i], " " if not T[1] else "Timeout" if T[1][i] == 0 else T[1][i], " " if not T[2] else "Timeout" if T[2][i] == 0 else T[2][i], " " if not T[3] else "Timeout" if T[3][i] == 0 else T[3][i]))

            # Process Case Name
            for i in range(len(case_name)):
                for j in range(len(case_name[i]) - 1):
                    if case_name[i][j] == ",":
                        case_name[i] = case_name[i][:j] + "\n" + case_name[i][j+2:]

            plt.clf()
            y_pos = np.arange(len(case_name))
            default_width = 0.2
            width = 0

            if T[0]:
                plt.bar(y_pos + width, T[0], width=default_width, label="T_S")
                width += default_width
            if T[1]:
                plt.bar(y_pos + width, T[1], width=default_width, label="T_I")
                width += default_width
            if T[2]:
                plt.bar(y_pos + width, T[2], width=default_width, label="T_B")
                width += default_width
            if T[3]:
                plt.bar(y_pos + width, T[3], width=default_width, label="T_B_Ann")
                width += default_width
            plt.grid(color='#95a5a6', linestyle='--', linewidth=1, axis='y', alpha=0.7)
            plt.xticks(y_pos, case_name)
            plt.ylabel('Circuit Size')
            plt.ylabel('Input')
            plt.legend(loc="upper left")
            plt.title(graph_name)

            with doc.create(Subsection('Graph for ' + graph_name)):
                with doc.create(Figure(position='htbp')) as plot:
                    plot.add_plot(width=NoEscape(r'1\textwidth'))
                    plot.add_caption('Graph for ' + graph_name)

    print("Finished " + graph_name)

doc.generate_pdf('Graphs', clean_tex=False)
rec_file.close()