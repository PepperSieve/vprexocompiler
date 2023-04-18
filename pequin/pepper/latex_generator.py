# Example List:
# 0: Find Min
# 1: Merging
# 2: Binary Search
# 3: KMP Search
# 4: Next Permutation
# 5: Dutch Flag
# 6: Recurrence Relations Sequence
# 7: Sum of Powers
# 8: 2D Convex Hull
# 9: MSC

import numpy as np

from pylatex import Document, Section, Subsection, Tabular, Math, TikZ, Axis, \
    Plot, Figure, Matrix, Alignat, NoEscape, SubFigure
from pylatex.utils import italic
import matplotlib.pyplot as plt
import os

# Init
graph_name_list = ["Find Min", "Merging", "Binary Search", "KMP Search", "Next Permutation", "Dutch Flag", "Recurrence Relations Sequence", "Sum of Powers", "2D Convex Hull", "MSC", "MST"]
short_names = {"Find Min": "FMin", "Merging": "Merg", "Binary Search": "BinS", "KMP Search": "KMP", "Next Permutation": "NPerm", "Dutch Flag": "DFlag", "Recurrence Relations Sequence": "RRSeq", "Sum of Powers": "SoP", "2D Convex Hull": "2DCH", "MSC": "MSC", "MST": "MST"}
method_name_list = {"T_S": 0, "T_I": 1, "T_B": 2, "T_SB": 0, "T_IA": 3, "T_BA": 4}
rec_file = open("result", "r")
line = rec_file.readline()
lineseg = line.split()

geometry_options = {"tmargin": "1cm", "lmargin": "1cm"}
doc = Document(geometry_options=geometry_options)

# --
# Process the data to obtain general improvement factor graph
ex_names = []
# Value: >  0 --> actual factor
#        = -1 --> time out
#        = -2 --> not applicable
init_ts = []
init_te = []
final_ts = []
final_te = []

# Record Values
while lineseg[0] != "End":
    # Parser
    graph_name = graph_name_list[int(lineseg[1])]
    #    T_S  T_I  T_B/T_SB  T_IA  T_BA
    T = [ [],  [],       [],   [],   []]

    line = rec_file.readline()
    lineseg = line.split()
    method_names = []
    
    ti_max = -1
    ti_max_ind = -1

    case_name = []
    while not lineseg[0] in ["Benchmark", "End"]:
        method_names.append(lineseg[0])
        method_ind = method_name_list[lineseg[0]]

        line = rec_file.readline()
        lineseg = line.split()
        cur_case = 0
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

            if method_ind == 1:
                case_name.append(tmp_name)
                T[method_ind].append(int(lineseg[-1]))
                cur_case += 1
                if T[1][-1] > ti_max:
                    ti_max = T[1][-1]
                    ti_max_ind = len(T[1]) - 1
            elif cur_case < len(case_name) and case_name[cur_case] == tmp_name:
                T[method_ind].append(int(lineseg[-1]))
                cur_case += 1
            elif tmp_name in case_name:
                while case_name[cur_case] != tmp_name:
                    T[method_ind].append(0)
                    cur_case += 1
                T[method_ind].append(int(lineseg[-1]))
                cur_case += 1


            line = rec_file.readline()
            lineseg = line.split()
        
        while cur_case < len(case_name):
            T[method_ind].append(0)
            cur_case += 1

    # Post-processing Examples
    ex_names.append(graph_name)
    if len(T[1]) == 0:
        errval = "No value for T_I of " + graph_name + " has been produced. Please extend timeout limit."
        raise ValueError(errval)
    if not T[0] or T[0][0] == 0:
        init_ts.append(-1)
    else:
        init_ts.append(T[0][0] / T[1][0])
    if "T_SB" in method_names:
        init_te.append(-2)
    elif not T[2] or T[2][0] == 0:
        init_te.append(-1)
    else:
        init_te.append(T[2][0] / T[1][0])

    if not T[0] or T[0][ti_max_ind] == 0:
        final_ts.append(-1)
    else:
        final_ts.append(T[0][ti_max_ind] / T[1][ti_max_ind])

    if "T_SB" in method_names:
        final_te.append(-2)
    elif not T[2] or T[2][ti_max_ind] == 0:
        final_te.append(-1)
    else:
        final_te.append(T[2][ti_max_ind] / T[1][ti_max_ind])

    # Individual Case Data
    if __name__ == '__main__':


        with doc.create(Section('Example: ' + graph_name)):

            with doc.create(Subsection('Table for ' + graph_name)):
                with doc.create(Tabular('l|llll')) as table:
                    table.add_row(("Input", "T_S", "T_I", "T_B", "T_B_Ann"))
                    table.add_hline()
                    for i in range(len(case_name)):
                        table.add_row((case_name[i], 
                                    " " if not T[0] else "Timeout" if T[0][i] == 0 else T[0][i], 
                                    " " if not T[1] else "Timeout" if T[1][i] == 0 else T[1][i], 
                                    " " if not T[2] else "Timeout" if T[2][i] == 0 else T[2][i], 
                                    " " if not T[3] else "Timeout" if T[3][i] == 0 else T[3][i]))

            # Process Case Name
            for i in range(len(case_name)):
                for j in range(len(case_name[i]) - 1):
                    if case_name[i][j] == ",":
                        case_name[i] = case_name[i][:j] + "\n" + case_name[i][j+2:]

    print("Finished " + graph_name)

# Grapher
# inf_h needs to be multiple of 0.2
inf_h = 1.8

if __name__ == '__main__':

    with doc.create(Section('Examples:')):

        with doc.create(Subsection('Table factor of improvement')):
            with doc.create(Tabular('l|llll')) as table:
                table.add_row(("Name", "T_S Init", "T_S Final", "T_B Init", "T_B Final"))
                table.add_hline()
                for i in range(len(ex_names)):
                    table.add_row((ex_names[i],
                                "N/A" if init_ts[i] == -2 else "Timeout" if init_ts[i] == -1 else round(init_ts[i], 3),
                                "N/A" if final_ts[i] == -2 else "Timeout" if final_ts[i] == -1 else round(final_ts[i], 3),
                                "N/A" if init_te[i] == -2 else "Timeout" if init_te[i] == -1 else round(init_te[i], 3),
                                "N/A" if final_te[i] == -2 else "Timeout" if final_te[i] == -1 else round(final_te[i], 3)))

        ex_names_full = ex_names[:]
        try:
            ex_names.remove("Merging")
        except:
            pass

        plt.clf()
        plt.figure(figsize=(14,4))
        plt.xlim(0, 0.2 + 1.2 * len(ex_names) + 0.4)
        plt.ylim(0, inf_h)
        # Compute y_pos
        j = 0
        y_pos = []
        for i in range(len(ex_names_full)):
            y_pos.append(0.4 + 1.2 * j)
            if not ex_names_full[i] in ["Merging"]:
                j += 1
        default_width = 0.2
        plt.axhline(y = 1, color = "red", linestyle = "-")

        label_ts = False
        label_te = False

        for i in range(len(ex_names_full)):
            if not ex_names_full[i] in ["Merging"]:
                # T_S Normal
                if final_ts[i] >= 0:
                    yp = y_pos[i] - 0.15
                    xp = final_ts[i]
                    if init_ts[i] >= 0:
                        plt.hlines(y = init_ts[i], xmin = yp - (default_width / 2), xmax = yp + (default_width / 2), color = "black", linestyle = "-")
                        plt.hlines(y = final_ts[i], xmin = yp - (default_width / 2), xmax = yp + (default_width / 2), color = "black", linestyle = "-")
                        if init_ts[i] - xp > 0:
                            error = [[0], [init_ts[i] - xp]]
                        else:
                            error = [[xp - init_ts[i]], [0]]
                        if abs(init_ts[i] - xp) >= 0.1:
                            plt.text(yp + 0.15, init_ts[i] - 0.02, str('%.2f' % init_ts[i]), fontsize = 10, color = 'gray')
                    if not label_ts:
                        plt.bar(yp, xp, yerr = error, width=default_width, color = 'orange', label = "T_S")
                        label_ts = True
                    else:
                        plt.bar(yp, xp, yerr = error, width=default_width, color = 'orange')
                    # if init_ts[i] >= 0:
                        # if init_ts[i] - xp > 0:
                            # if init_ts[i] < inf_h and abs(init_ts[i] - xp) >= 0.1:
                                # plt.hlines(x = yp, y = xp + 0.1, dx = 0, dy = -0.1, length_includes_head = True, head_width=0.15, head_length=0.06, color = 'black', overhang = 1)
                        #else:
                            # if init_ts[i] < inf_h and abs(init_ts[i] - xp) >= 0.1:
                                # plt.arrow(x = yp, y = xp - 0.1, dx = 0, dy = 0.1, length_includes_head = True, head_width=0.15, head_length=0.06, color = 'black', overhang = 1)
                    xp_height = xp if xp < inf_h - 0.08 else inf_h - 0.08
                    plt.text(yp + 0.15, xp_height - 0.02, str('%.2f' % xp), fontsize = 10, color = 'black')
                # T_S with T/O
                elif final_ts[i] == -1:
                    # Set bar height to inf_h
                    yp = y_pos[i] - 0.15
                    xp = inf_h
                    if init_ts[i] >= 0:
                        plt.hlines(y = init_ts[i], xmin = yp - (default_width / 2), xmax = yp + (default_width / 2), color = "black", linestyle = "-")
                        plt.hlines(y = final_ts[i], xmin = yp - (default_width / 2), xmax = yp + (default_width / 2), color = "black", linestyle = "-")
                        if init_ts[i] - xp > 0:
                            error = [[0], [init_ts[i] - xp]]
                        else:
                            error = [[xp - init_ts[i]], [0]]
                        if init_ts[i] < inf_h and abs(init_ts[i] - xp) >= 0.1:
                            if ex_names_full[i] == "MSC":
                                plt.text(yp - 0.52, init_ts[i] - 0.02, str('%.2f' % init_ts[i]), fontsize = 10, color = 'gray')
                            else:
                                plt.text(yp + 0.15, init_ts[i] - 0.02, str('%.2f' % init_ts[i]), fontsize = 10, color = 'gray')
                    if not label_ts:
                        plt.bar(yp, xp, yerr = error, width=default_width, color = 'orange', label = "T_S")
                        label_ts = True
                    else:
                        plt.bar(yp, xp, yerr = error, width=default_width, color = 'orange')
                    # if init_ts[i] >= 0:
                        # if init_ts[i] - xp > 0:
                            # if init_ts[i] < inf_h and abs(init_ts[i] - xp) >= 0.08:
                                # plt.arrow(x = yp, y = xp + 0.1, dx = 0, dy = -0.1, length_includes_head = True, head_width=0.15, head_length=0.06, color = 'black', overhang = 1)
                        # else:
                            # if init_ts[i] < inf_h and abs(init_ts[i] - xp) >= 0.08:
                                # plt.arrow(x = yp, y = xp - 0.1, dx = 0, dy = 0.1, length_includes_head = True, head_width=0.15, head_length=0.06, color = 'black', overhang = 1)
                    plt.text(yp + 0.15, xp - 0.1, "T/O", fontsize = 10, color = 'black')
                # T_E
                if final_te[i] >= 0:
                    yp = y_pos[i] + 0.15
                    xp = final_te[i]
                    if init_te[i] >= 0:
                        plt.hlines(y = init_te[i], xmin = yp - (default_width / 2), xmax = yp + (default_width / 2), color = "black", linestyle = "-")
                        plt.hlines(y = final_te[i], xmin = yp - (default_width / 2), xmax = yp + (default_width / 2), color = "black", linestyle = "-")
                        if init_te[i] - xp > 0:
                            error = [[0], [init_te[i] - xp]]
                        else:
                            error = [[xp - init_te[i]], [0]]
                        if init_te[i] < inf_h and abs(init_te[i] - xp) >= 0.08:
                            plt.text(yp + 0.15, init_te[i] - 0.02, str('%.2f' % init_te[i]), fontsize = 10, color = 'gray')
                    if not label_te:
                        plt.bar(yp, xp, yerr = error, width=default_width, color = '#069AF3', label = "T_E")
                        label_te = True
                    else:
                        plt.bar(yp, xp, yerr = error, width=default_width, color = '#069AF3')
                    # if init_te[i] >= 0:
                        # if init_te[i] - xp > 0:
                            # if init_te[i] < inf_h and abs(init_te[i] - xp) >= 0.08:
                                # plt.arrow(x = yp, y = xp + 0.1, dx = 0, dy = -0.1, length_includes_head = True, head_width=0.15, head_length=0.06, color = 'black', overhang = 1)
                        # else:
                            # if init_te[i] < inf_h and abs(init_te[i] - xp) >= 0.08:
                                # plt.arrow(x = yp, y = xp - 0.1, dx = 0, dy = 0.1, length_includes_head = True, head_width=0.15, head_length=0.06, color = 'black', overhang = 1)
                    plt.text(yp + 0.15, xp - 0.02, str('%.3f' % init_te[i]) if str('%.2f' % init_te[i]) == "0.00" else str('%.2f' % xp), fontsize = 10, color = 'black')
        plt.grid(color='#95a5a6', linestyle='--', linewidth=1, axis='y', alpha=0.7)

        y_pos = [0.4 + 1.2 * i for i in range(len(ex_names))]
        plt.xticks(y_pos, (short_names[i] for i in ex_names))
        yt = [str('%.1f' % (0.2 * i)) for i in range(int(5 * inf_h) + 1)]
        yt[0] = "0"
        # yt[-1] = "$\infty$"
        plt.yticks([0.2 * i for i in range(len(yt))], yt)
        plt.ylabel('Relative Constraint Size\n(Lower is Better)')
        plt.legend(loc="upper right")

        plt.savefig('graph1.png')
        plt.savefig('graph1.pdf', dpi=None, 
                          facecolor='w',                                                
                          edgecolor='w',
                          orientation='portrait',                                 
                          format=None,                                
                          transparent=False,     
                          bbox_inches=None,                
                          pad_inches=0.1,                                      
                          metadata=None)

        with doc.create(Subsection('Graphs:')):
            with doc.create(Figure(position='htbp')) as plot:
                plot.add_plot(width=NoEscape(r'1.1\textwidth'))
                plot.add_caption('Graph of Relative Constraint Size')

rec_file.close()

# --
# Process the data AGAIN to generate graphs for asymptotic examples
rec_file = open("result", "r")
line = rec_file.readline()
lineseg = line.split()

with doc.create(Figure(position='htbp')) as sup_plot:
    # Record Values
    while lineseg[0] != "End":
        # Parser
        graph_name = graph_name_list[int(lineseg[1])]
        #    T_S  T_I  T_B/T_SB  T_IA  T_BA
        T = [ [],  [],       [],   [],   []]

        line = rec_file.readline()
        lineseg = line.split()
        method_names = []
    
        ti_max = -1
        ti_max_ind = -1

        case_name = []
        while not lineseg[0] in ["Benchmark", "End"]:
            method_names.append(lineseg[0])
            method_ind = method_name_list[lineseg[0]]

            line = rec_file.readline()
            lineseg = line.split()
            cur_case = 0
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

                if graph_name in ["Binary Search"]:
                    if method_ind == 3:
                        case_name.append(tmp_name)
                        T[method_ind].append(int(lineseg[-1]))
                        cur_case += 1
                        if T[3][-1] > ti_max:
                            ti_max = T[3][-1]
                            ti_max_ind = len(T[3]) - 1
                    elif cur_case < len(case_name) and case_name[cur_case] == tmp_name:
                        T[method_ind].append(int(lineseg[-1]))
                        cur_case += 1
                    elif tmp_name in case_name:
                        while case_name[cur_case] != tmp_name:
                            T[method_ind].append(0)
                            cur_case += 1
                        T[method_ind].append(int(lineseg[-1]))
                        cur_case += 1
                else:
                    if method_ind == 1:
                        case_name.append(tmp_name)
                        T[method_ind].append(int(lineseg[-1]))
                        cur_case += 1
                        if T[1][-1] > ti_max:
                            ti_max = T[1][-1]
                            ti_max_ind = len(T[1]) - 1
                    elif cur_case < len(case_name) and case_name[cur_case] == tmp_name:
                        T[method_ind].append(int(lineseg[-1]))
                        cur_case += 1
                    elif tmp_name in case_name:
                        while case_name[cur_case] != tmp_name:
                            T[method_ind].append(0)
                            cur_case += 1
                        T[method_ind].append(int(lineseg[-1]))
                        cur_case += 1

                line = rec_file.readline()
                lineseg = line.split()
        
            while cur_case < len(case_name):
                T[method_ind].append(0)
                cur_case += 1

        # Post-processing Examples: Merging
        if graph_name in ["Merging"]:
            inf_h = 1.2
            ts_factor = []
            te_factor = []
            if len(T[1]) == 0:
                errval = "No value for T_I of " + graph_name + " has been produced. Please extend timeout limit."
                raise ValueError(errval)
        
            for j in range(len(case_name[0]) - 1):
                if case_name[0][j] == ",":
                    prefix = case_name[0][:j]
                    break        

            case_num = 0
            for i in range(len(T[1])):
                if case_name[i][:len(prefix)] == prefix:
                    if not T[0] or T[0][i] == 0:
                        ts_factor.append(inf_h)
                    else:
                        ts_factor.append(T[0][i] / T[1][i])
                    if "T_SB" in method_names:
                        te_factor.append(-2)
                    elif not T[2] or T[2][i] == 0:
                        te_factor.append(inf_h)
                    else:
                        te_factor.append(T[2][i] / T[1][i])
                    case_num += 1

            # Only plot the first 5
            case_name = case_name[:5]
            ts_factor = ts_factor[:5]
            te_factor = te_factor[:5]

            # Process Case Name
            for i in range(len(case_name)):
                for j in range(len(case_name[i]) - 1):
                    if case_name[i][j] == "L":
                        case_name[i] = case_name[i][j:]
                        break

            plt.clf()
            plt.figure(figsize=(6,4))
            y_pos = np.arange(0, 2 * (len(case_name)), 2)
            plt.ylim(0, inf_h)
            plt.xlim(-0.8, y_pos[-1] + 2)
            default_width = 0.4
            width = 0
            plt.axhline(y = 1, color = "red", linestyle = "-")

            label_ts = False
            label_te = False

            # Generate bars
            if ts_factor[0] != -2:
                plt.bar(y_pos + width - 0.15, ts_factor, width=default_width, color='orange', label="T_S")
                for i in range(len(ts_factor)):
                    plt.text(y_pos[i] + width + 0.2, ts_factor[i] - 0.02, str('%.2f' % ts_factor[i]), fontsize = 11, color = 'black')
                width += default_width
            if te_factor[0] != -2:
                plt.bar(y_pos + width - 0.05, te_factor, width=default_width, color='#069AF3', label="T_E")
                for i in range(len(te_factor)):
                    plt.text(y_pos[i] + width + 0.2, te_factor[i] - 0.02, str('%.2f' % te_factor[i]), fontsize = 11, color = 'black')
                width += default_width

            plt.grid(color='#95a5a6', linestyle='--', linewidth=1, axis='y', alpha=0.7)
            plt.axhline(y = 1, color = "red", linestyle = "-")
            plt.xticks(y_pos, case_name[:case_num], fontsize = 11)
            plt.yticks(fontsize = 11)
            plt.ylabel('Relative Constraint Size\n(Lower is Better)', fontsize = 11)
            plt.legend(loc="upper right")

            plt.savefig('graph2.png')
            plt.savefig('graph2.pdf', dpi=None, 
                          facecolor='w',                                                
                          edgecolor='w',
                          orientation='portrait',                                 
                          format=None,                                
                          transparent=False,     
                          bbox_inches=None,                
                          pad_inches=0.1,                                      
                          metadata=None)

            with doc.create(SubFigure(position='b')) as plot:
                plot.add_plot(width=NoEscape(r'0.8\textwidth'))
                plot.add_caption('Graph of ' + graph_name)

        # Post-processing Examples: Binary Search
        if graph_name in ["Binary Search"]:
            inf_h = 1.2
            ts_factor = []
            tea_factor = []
            if len(T[3]) == 0:
                errval = "No value for T_I of " + graph_name + " has been produced. Please extend timeout limit."
                raise ValueError(errval)
        
            for i in range(len(T[3])):
                if not T[0] or T[0][i] == 0:
                    ts_factor.append(inf_h)
                else:
                    ts_factor.append(T[0][i] / T[3][i])
                if not T[4]:
                    tea_factor.append(-2)
                elif T[4][i] == 0:
                    tea_factor.append(inf_h)
                else:
                    tea_factor.append(T[4][i] / T[3][i])

            # Only plot the first 5
            case_name = [i.lower() for i in case_name[:5]]
            ts_factor = ts_factor[:5]
            tea_factor = tea_factor[:5]

            plt.clf()
            plt.figure(figsize=(6,4))
            y_pos = np.arange(0, 2 * (len(case_name)), 2)
            plt.ylim(0, inf_h)
            plt.xlim(-0.8, y_pos[-1] + 3)
            default_width = 0.4
            width = 0
            plt.axhline(y = 1, color = "red", linestyle = "-")

            label_ts = False
            label_te = False

            if ts_factor[0] != -2:
                plt.bar(y_pos + width - 0.3, ts_factor, width=default_width, color='orange', label="T_S")
                for i in range(len(ts_factor)):
                    plt.text(y_pos[i] + width, inf_h - 0.06, str('%.2f' % ts_factor[i]) if ts_factor[i] != inf_h else "T/O", fontsize = 11, color = 'black')
                width += default_width
            if tea_factor[0] != -2:
                plt.bar(y_pos + width - 0.1, tea_factor, width=default_width, color='#069AF3', label="T_E")
                for i in range(len(tea_factor)):
                    plt.text(y_pos[i] + width + 0.16, tea_factor[i] - 0.008, str('%.2f' % tea_factor[i]), fontsize = 11, color = 'black')
                width += default_width

            plt.grid(color='#95a5a6', linestyle='--', linewidth=1, axis='y', alpha=0.7)
            plt.axhline(y = 1, color = "red", linestyle = "-")
            plt.xticks(y_pos, case_name, fontsize = 11)
            plt.yticks(fontsize = 11)
            # plt.ylabel('Relative Constraint Size\n(Lower is Better)')
            plt.legend(loc="upper right")

            plt.savefig('graph3.png')
            plt.savefig('graph3.pdf', dpi=None, 
                          facecolor='w',                                                
                          edgecolor='w',
                          orientation='portrait',                                 
                          format=None,                                
                          transparent=False,     
                          bbox_inches=None,                
                          pad_inches=0.1,                                      
                          metadata=None)

            with doc.create(SubFigure(position='b')) as plot:
                plot.add_plot(width=NoEscape(r'0.8\textwidth'))
                plot.add_caption('Graph of ' + graph_name)


        # Post-processing Examples: 2D Convex Hull
        if graph_name in ["2D Convex Hull"]:
            inf_h = 0.1
            ts_factor = []
            tea_factor = []
            if len(T[1]) == 0:
                errval = "No value for T_I of " + graph_name + " has been produced. Please extend timeout limit."
                raise ValueError(errval)
        
            for i in range(len(T[1])):
                if not T[0] or T[0][i] == 0:
                    ts_factor.append(inf_h)
                else:
                    ts_factor.append(T[0][i] / T[1][i])
                if not T[4]:
                    tea_factor.append(-2)
                elif T[4][i] == 0:
                    tea_factor.append(inf_h)
                else:
                    tea_factor.append(T[4][i] / T[1][i])

            # Only plot the first 5
            case_name = [i.lower() for i in case_name[:5]]
            ts_factor = ts_factor[:5]
            tea_factor = tea_factor[:5]

            plt.clf()
            plt.figure(figsize=(5,4))
            y_pos = np.arange(0, 2 * len(case_name), 2)
            plt.ylim(0, inf_h)
            plt.xlim(-0.8, y_pos[-1] + 3.6)
            default_width = 0.4
            width = 0
            plt.axhline(y = 1, color = "red", linestyle = "-")

            label_ts = False
            label_te = False

            if ts_factor[0] != -2:
                plt.bar(y_pos + width - 0.3, ts_factor, width=default_width, color='orange', label="T_S")
                for i in range(len(ts_factor)):
                    plt.text(y_pos[i] + width, inf_h - 0.006, str('%.2f' % ts_factor[i]) if ts_factor[i] != inf_h else "T/O", fontsize = 11, color = 'black')
                width += default_width
            if tea_factor[0] != -2:
                plt.bar(y_pos + width - 0.1, tea_factor, width=default_width, color='#069AF3', label="T_E")
                for i in range(len(tea_factor)):
                    plt.text(y_pos[i] + width + 0.06, tea_factor[i] + 0.001, str('%.3f' % tea_factor[i])[1:], fontsize = 11, color = 'black')
                width += default_width

            plt.grid(color='#95a5a6', linestyle='--', linewidth=1, axis='y', alpha=0.7)
            plt.axhline(y = 1, color = "red", linestyle = "-")
            plt.xticks(y_pos, case_name, fontsize = 11)
            plt.yticks(fontsize = 11)
            # plt.ylabel('Relative Constraint Size\n(Lower is Better)')
            plt.legend(loc="upper right")

            plt.savefig('graph4.png')
            plt.savefig('graph4.pdf', dpi=None, 
                          facecolor='w',                                                
                          edgecolor='w',
                          orientation='portrait',                                 
                          format=None,                                
                          transparent=False,     
                          bbox_inches=None,                
                          pad_inches=0.1,                                      
                          metadata=None)

            with doc.create(SubFigure(position='b')) as plot:
                plot.add_plot(width=NoEscape(r'0.8\textwidth'))
                plot.add_caption('Graph of ' + graph_name)

        print("Finished " + graph_name)
    sup_plot.add_caption('Graphs of Asymptotic Examples')

doc.generate_pdf('Graphs', clean_tex=False)
rec_file.close()