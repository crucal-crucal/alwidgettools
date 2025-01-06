@echo off
setlocal

set NEW_FILE=new.ts
set OLD_FILE=zh_CN.ts

rem 使用 lupdate 生成新的 ts文件
lupdate ..\..\src -ts %NEW_FILE%

if exist %OLD_FILE% (
    rem 合并新的 ts文件和旧的 ts文件
    lconvert -i %NEW_FILE% -i %OLD_FILE% -o merged.ts
    rem 新生成的 ts文件
    del %NEW_FILE%
    rem 重命名合并后的 ts文件
    move /Y merged.ts %OLD_FILE%
    echo Merge and rename completed.
) else (
    rem 直接重命名新的 ts文件为旧的 ts文件
    move /Y %NEW_FILE% %OLD_FILE%
    echo New file renamed as old file.
)

