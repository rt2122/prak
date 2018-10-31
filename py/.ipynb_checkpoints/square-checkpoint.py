{
 "cells": [
  {
   "cell_type": "code",
   "execution_count": 10,
   "metadata": {
    "scrolled": true
   },
   "outputs": [
    {
     "name": "stdout",
     "output_type": "stream",
     "text": [
      "+ - - + - - +\n",
      "/     /     /\n",
      "/     /     /\n",
      "+ - - + - - +\n",
      "/     /     /\n",
      "/     /     /\n",
      "+ - - + - - +\n"
     ]
    }
   ],
   "source": [
    "def sq(n):\n",
    "    def print_hor():\n",
    "        print(\"+ \" + \"- \" * n + \"+ \" + \"- \" * n + \"+\")\n",
    "    def print_vert():\n",
    "        print((\"/ \" + \"  \" * n + \"/ \" + \"  \" * n + \"/\\n\") * n, end = \"\")\n",
    "    print_hor()\n",
    "    print_vert()\n",
    "    print_hor()\n",
    "    print_vert()\n",
    "    print_hor()\n",
    "    \n",
    "sq(2)"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": null,
   "metadata": {},
   "outputs": [],
   "source": []
  }
 ],
 "metadata": {
  "kernelspec": {
   "display_name": "Python 3",
   "language": "python",
   "name": "python3"
  },
  "language_info": {
   "codemirror_mode": {
    "name": "ipython",
    "version": 3
   },
   "file_extension": ".py",
   "mimetype": "text/x-python",
   "name": "python",
   "nbconvert_exporter": "python",
   "pygments_lexer": "ipython3",
   "version": "3.7.0"
  }
 },
 "nbformat": 4,
 "nbformat_minor": 2
}
