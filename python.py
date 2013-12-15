def sum3(nums):
         return sum(nums)

def rotate_left3(nums):
        return nums[1:] + [nums[0]]

def max_end3(nums):
        m = max(nums[0], nums[-1])
        return [m for x in nums]

def make_ends(nums):
        return [nums[0], nums[-1]]

def has23(nums):
        return 2 in nums or 3 in nums

def count_evens(nums):
        return len([x for x in nums if\
        x % 2 == 0])

def sum13(nums):
        sum = 0
        for i in range(0, len(nums)):
                if nums[i] == 13:
                        nums[i - len(nums) + 1] = 0
                else:
                        sum += nums[i]
        return sum

def big_diff(nums):
        return max(nums) - min(nums)

def sum67(nums):
        count = 0
        run = 1

        for x in nums:
                if run == 1:
                        if x == 6:
                                run = 0
                        else:
                                count += x
                elif x == 7:
                        run = 1
        return count

def centered_average(nums):
        return ( sum(nums) - max(nums) - min(nums)) / (len(nums) - 2)
		
def has22(nums):
        for i in range (0, len(nums) - 1):
                if nums[i] == 2 and nums [i + 1] == 2:
                        return True
        return False

def extra_end(str):
        return str[-2:] * 3

def without_end(str):
        return str[1:-1]

def double_char(str):
        s = ""
        for x in str:
                s += x * 2
        return s

def count_code(str):
        count = 0
        for i in range(0, len(str) - 3):
                if str[i] == 'c' and str[i+1] == 'o' and \
                str[i+3] == 'e':
                        count += 1
        return count

def count_hi(str):
        return str.count('hi')

def end_other(nums):
        a = a.lower()
        b = b.lower()
        return a.endswith(b) or b.endswith(a)

def cat_dog(str):
        return str.count('cat') == str.count('dog')

def xyz_there(str):
        return str.count('xyz') - \
        str.count('.xyz') > 0

def wordCount(str):
        return [len(str), str.count('\n') + 1, \
        len(str.split())]

def myCount(str):
        return [str.count(c) for c in "0123456789 \t\n"]

