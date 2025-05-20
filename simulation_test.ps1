# 要尝试的 TOTAL_UTILIZATION 值列表
# $utilizations = @(2.2, 2.4, 2.6, 2.8, 3.0)
$utilizations = @(0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8)
# $utilizations = @(4.0, 4.2, 4.4, 4.6, 4.8, 5.0 ,5.2, 5.4, 5.6, 5.8)
# $utilizations = @(0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6)

# 要尝试的 NUMBER_TASK 值列表
# $NTask = @(9,8,7)

# main.h 文件路径
$mainHeaderPath = "main.h"

# 输出目录
$outputDir = "results"

# 可执行文件名
$exeName = "main.exe"

# 所有源文件
$sourceFiles = @(
    "main.c",
    "DA_LC_Test.c",
    "Simulator_1.c",
    "Task_generate.c",
    "DA_LC_OPA.c",
    "RTA_DP_OPA.c",
    "RTA_DP.c",
    "G_MP.c"
)

# 创建输出目录
if (-not (Test-Path $outputDir)) {
    New-Item -ItemType Directory -Path $outputDir | Out-Null
}

foreach ($util in $utilizations) {

    $valFormatted = "{0:F1}" -f $util
    $valFilename = $valFormatted.Replace('.', '_')

    Write-Host "set TOTAL_UTILIZATION = $util"

    # 替换 main.h 中的 TOTAL_UTILIZATION 宏定义
    (Get-Content $mainHeaderPath) |
        ForEach-Object {
            $_ -replace "#define TOTAL_UTILIZATION\s+[0-9.]+", "#define TOTAL_UTILIZATION $valFormatted"
        } |
        Set-Content $mainHeaderPath

    # 删除旧的可执行文件
    if (Test-Path $exeName) {
        Remove-Item $exeName
    }

    # 调用 cl 编译
    $compileCmd = "cl /nologo /Fe$exeName " + ($sourceFiles -join ' ')
    Invoke-Expression $compileCmd

    # 检查是否编译成功并运行程序
    # if (Test-Path $exeName) {
    #     & ".\$exeName" > "$outputDir\output_$($util.ToString().Replace('.', '_')).txt"

    # } 

        if (Test-Path $exeName) {
        # & ".\$exeName" > "$outputDir\output_$valFilename.txt"

            $outputFile = "$outputDir\all_results.txt"

# 添加分隔行和参数信息
Add-Content $outputFile "`n===== TOTAL_UTILIZATION = $valFormatted ====="

# 执行程序并追加输出
& ".\$exeName" | Out-String | Add-Content $outputFile


    }

    # 清理 cl 生成的中间文件（可选）
    # Remove-Item *.obj, *.pdb -ErrorAction SilentlyContinue
}
