# **************************************************************************** #
#                                COMPARE PARSING                               #
# ****************************************************************************
#Case 11
#42_minishell_tester/cmds/mand/0_compare_parsing.sh:26
/bin/echo $"HOME"$USER

#Case 12
#42_minishell_tester/cmds/mand/0_compare_parsing.sh:28
/bin/echo $"HOM"E$USER

#Case 14
#42_minishell_tester/cmds/mand/0_compare_parsing.sh:32
/bin/echo $"HOME"

#Case 15
#42_minishell_tester/cmds/mand/0_compare_parsing.sh:34
/bin/echo $"42$"

#Case 89
#42_minishell_tester/cmds/mand/0_compare_parsing.sh:182
# works in minishell

# **************************************************************************** #
#                                PARSING HELL                                  #
# **************************************************************************** #

#Case 2
#42_minishell_tester/cmds/mand/10_parsing_hell.sh:7
>">" asd

#Case 3
#42_minishell_tester/cmds/mand/10_parsing_hell.sh:10
# works in minishell

#Case 12 !!!!! --------- no need to expand now, hope to pass without it
#42_minishell_tester/cmds/mand/10_parsing_hell.sh:32
cat << $USER
why
not
$USER

#Case 13
#42_minishell_tester/cmds/mand/10_parsing_hell.sh:37
#parsing
export T=">>"
$T lol

#Case 14 !!!!! --------- no need to expand now, hope to pass without it
#42_minishell_tester/cmds/mand/10_parsing_hell.sh:40
cat << "$USER"
why
not
$USER

#Case 15 !!!!! --------- no need to expand now, hope to pass without it
#42_minishell_tester/cmds/mand/10_parsing_hell.sh:45
cat << "$USER"
why
not
$USER

#Case 20
#42_minishell_tester/cmds/mand/10_parsing_hell.sh:66
# works in minishell

#Case 21
#42_minishell_tester/cmds/mand/10_parsing_hell.sh:69
# works in minishell

#Case 22
#42_minishell_tester/cmds/mand/10_parsing_hell.sh:72
# works in minishell

#Case 24
#42_minishell_tester/cmds/mand/10_parsing_hell.sh:78
# works in minishell

#Case 25
#42_minishell_tester/cmds/mand/10_parsing_hell.sh:81
# works in minishell

#Case 26
#42_minishell_tester/cmds/mand/10_parsing_hell.sh:84
# works in minishell

#Case 27
#42_minishell_tester/cmds/mand/10_parsing_hell.sh:87
# works in minishell

#Case 28
#42_minishell_tester/cmds/mand/10_parsing_hell.sh:90
# works in minishell


#Case 29
#42_minishell_tester/cmds/mand/10_parsing_hell.sh:93
# works in minishell


#Case 30
#42_minishell_tester/cmds/mand/10_parsing_hell.sh:96
export T="-n test1 -n test 2"
echo $T

#Case 31
#42_minishell_tester/cmds/mand/10_parsing_hell.sh:99
# works in minishell

# **************************************************************************** #
#                                   BUILTINS                                   #
# **************************************************************************** #

#Case 13
#42_minishell_tester/cmds/mand/1_builtins.sh:31
# works in minishell


#Case 14
#42_minishell_tester/cmds/mand/1_builtins.sh:33
# works in minishell

#Case 16
#42_minishell_tester/cmds/mand/1_builtins.sh:37
echo $"HOME"

#Case 17
#42_minishell_tester/cmds/mand/1_builtins.sh:39
echo $"HOME"

#Case 73
#42_minishell_tester/cmds/mand/1_builtins.sh:152
# works in minishell

#Case 91
#42_minishell_tester/cmds/mand/1_builtins.sh:190
# works in minishell

#Case 92
#42_minishell_tester/cmds/mand/1_builtins.sh:194
# works in minishell

#Case 94
#42_minishell_tester/cmds/mand/1_builtins.sh:201
# works in minishell


#Case 95
#42_minishell_tester/cmds/mand/1_builtins.sh:205
# works in minishell


#Case 96
#42_minishell_tester/cmds/mand/1_builtins.sh:209
# works in minishell

#Case 97
#42_minishell_tester/cmds/mand/1_builtins.sh:213
# works in minishell

#Case 98
#42_minishell_tester/cmds/mand/1_builtins.sh:217
# works in minishell

#Case 99
#42_minishell_tester/cmds/mand/1_builtins.sh:221
# works in minishell

#Case 100
#42_minishell_tester/cmds/mand/1_builtins.sh:225
# works in minishell

#Case 101
#42_minishell_tester/cmds/mand/1_builtins.sh:229
# works in minishell

#Case 102 !!!!! --------- no need to expand now, hope to pass without it
#42_minishell_tester/cmds/mand/1_builtins.sh:233
# works in minishell

#Case 105 !!!!! --------- no need to expand now, hope to pass without it
#42_minishell_tester/cmds/mand/1_builtins.sh:239
# works in minishell

#Case 107 !!!!! --------- no need to expand now, hope to pass without it
#42_minishell_tester/cmds/mand/1_builtins.sh:243
# works in minishell

#Case 114
#42_minishell_tester/cmds/mand/1_builtins.sh:257
# works in minishell

#Case 115
#42_minishell_tester/cmds/mand/1_builtins.sh:261
# works in minishell

#Case 116
#42_minishell_tester/cmds/mand/1_builtins.sh:265
# works in minishell

#Case 117
#42_minishell_tester/cmds/mand/1_builtins.sh:269
# works in minishell

#Case 118
#42_minishell_tester/cmds/mand/1_builtins.sh:273
# works in minishell

#Case 119 !!!!! --------- no need to expand now, hope to pass without it
#42_minishell_tester/cmds/mand/1_builtins.sh:278
pwd
cd ?
pwd

#Case 120
#42_minishell_tester/cmds/mand/1_builtins.sh:282
# works in minishell

#Case 121
#42_minishell_tester/cmds/mand/1_builtins.sh:286
# works in minishell

#Case 122
#42_minishell_tester/cmds/mand/1_builtins.sh:290
# works in minishell

#Case 123
#42_minishell_tester/cmds/mand/1_builtins.sh:294
# works in minishell

#Case 124
#42_minishell_tester/cmds/mand/1_builtins.sh:298
# works in minishell

#Case 125
#42_minishell_tester/cmds/mand/1_builtins.sh:302
# works in minishell

#Case 126
#42_minishell_tester/cmds/mand/1_builtins.sh:306
# works in minishell

#Case 127
#42_minishell_tester/cmds/mand/1_builtins.sh:310
# works in minishell

#Case 128
#42_minishell_tester/cmds/mand/1_builtins.sh:314
# works in minishell

#Case 129
#42_minishell_tester/cmds/mand/1_builtins.sh:320
# works in minishell


#Case 130
#42_minishell_tester/cmds/mand/1_builtins.sh:324
# works in minishell

#Case 131
#42_minishell_tester/cmds/mand/1_builtins.sh:328
# works in minishell

#Case 132
#42_minishell_tester/cmds/mand/1_builtins.sh:333
# works in minishell

#Case 133
#42_minishell_tester/cmds/mand/1_builtins.sh:337
# works in minishell

#Case 134
#42_minishell_tester/cmds/mand/1_builtins.sh:342
# works in minishell

#Case 135
#42_minishell_tester/cmds/mand/1_builtins.sh:347
# works in minishell

#Case 137
#42_minishell_tester/cmds/mand/1_builtins.sh:354
# works in minishell

#Case 138
#42_minishell_tester/cmds/mand/1_builtins.sh:356
# works in minishell

#Case 139
#42_minishell_tester/cmds/mand/1_builtins.sh:358
# works in minishell

#Case 224 !!!!! --------- no need to expand now, hope to pass without it
#42_minishell_tester/cmds/mand/1_builtins.sh:552
unset TES;T

# **************************************************************************** #
#                                  PIPELINES                                   #
# **************************************************************************** #

#Case 3
#42_minishell_tester/cmds/mand/1_pipelines.sh:8
# works in minishell

#Case 25
#42_minishell_tester/cmds/mand/1_pipelines.sh:55
# works in minishell

#Case 26
#42_minishell_tester/cmds/mand/1_pipelines.sh:59
# works in minishell

#Case 27
#42_minishell_tester/cmds/mand/1_pipelines.sh:68
# works in minishell

#Case 28
#42_minishell_tester/cmds/mand/1_pipelines.sh:84
# works in minishell

#Case 29
#42_minishell_tester/cmds/mand/1_pipelines.sh:96
# parsing error
ls | cat << stop | ls -la | cat << stop1 | ls | cat << stop2 | ls -la > > out | cat << stop3
fe
wf
fwe
f
stop
dw
stop1
dwasd
stop2
cat Makefile | ls > out
cat out
/bin/rm -f out

#Case 30
#42_minishell_tester/cmds/mand/1_pipelines.sh:110
# works in minishell

#Case 33
#42_minishell_tester/cmds/mand/1_pipelines.sh:138
# parsing error
|ls | ls

#Case 36
#42_minishell_tester/cmds/mand/1_pipelines.sh:144


####################################################################
#                         REDIRECTIONS                             #
####################################################################

#Case 59
#42_minishell_tester/cmds/mand/1_redirs.sh:121
# works in minishell

#Case 60
# 42_minishell_tester/cmds/mand/1_redirs.sh:125
# parsing
/bin/echo "cat this text\n" > tmp_redir_out
cat <tmp_redir_out

# Case 65
# 42_minishell_tester/cmds/mand/1_redirs.sh:138
# works in minishell

# Case 66
# 42_minishell_tester/cmds/mand/1_redirs.sh:142
# works in minishell

# Case 67
# 42_minishell_tester/cmds/mand/1_redirs.sh:153
# works in minishell

# Case 80
# 42_minishell_tester/cmds/mand/1_redirs.sh:186
# works in minishell

# Case 81
# 42_minishell_tester/cmds/mand/1_redirs.sh:186
# works in minishell

# Case 82
# 42_minishell_tester/cmds/mand/1_redirs.sh:204
# works in minishell

# Case 83
# 42_minishell_tester/cmds/mand/1_redirs.sh:208
# works in minishell

# Case 84 !!!!! --------- no need to expand now, hope to pass without it
# 42_minishell_tester/cmds/mand/1_redirs.sh:217

cat << hello
$USER
$NOVAR
$HOME
hello

# Case 85
# 42_minishell_tester/cmds/mand/1_redirs.sh:223
# works in minishell

# Case 86
# 42_minishell_tester/cmds/mand/1_redirs.sh:229
# works in minishell

# Case 87
# 42_minishell_tester/cmds/mand/1_redirs.sh:235
# works in minishell

# Case 88 !!!!! --------- no need to expand now, hope to pass without it
# 42_minishell_tester/cmds/mand/1_redirs.sh:241
cat << 'lim'
$USER
$NOVAR
$HOME
lim

# Case 89
# 42_minishell_tester/cmds/mand/1_redirs.sh:247
# works in minishell

# Case 90
# 42_minishell_tester/cmds/mand/1_redirs.sh:250
# works in minishell

# Case 91
# 42_minishell_tester/cmds/mand/1_redirs.sh:253
# works in minishell


  # **************************************************************************** #
  #                                     SCMD                                     #
  # **************************************************************************** #

# Case 7
# 42_minishell_tester/cmds/mand/1_redirs.sh:24
# works in minishell

# Case 8
# 42_minishell_tester/cmds/mand/1_redirs.sh:27
# works in minishell

# **************************************************************************** #
#                                  PATH FAILS                                  #
# **************************************************************************** #

# Case 1
# 42_minishell_tester/cmds/mand/1_redirs.sh:5
# works in minishell

# Case 2
# 42_minishell_tester/cmds/mand/1_redirs.sh:12
# works in minishell

# Case 3
# 42_minishell_tester/cmds/mand/1_redirs.sh:21
# works in minishell

# Case 5
# 42_minishell_tester/cmds/mand/1_redirs.sh:26
# works in minishell

# Case 8
# 42_minishell_tester/cmds/mand/1_redirs.sh:32
# works in minishell

# Case 9
# 42_minishell_tester/cmds/mand/1_redirs.sh:35
# works in minishell

# Case 10
# 42_minishell_tester/cmds/mand/1_redirs.sh:39
# works in minishell

# Case 11
# 42_minishell_tester/cmds/mand/1_redirs.sh:47
# works in minishell

# Case 13
# 42_minishell_tester/cmds/mand/1_redirs.sh:68
# works in minishell