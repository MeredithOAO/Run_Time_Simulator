# 要尝试的 TOTAL_UTILIZATION 值列表
$utilizations = @(1.0, 1.5, 2.0, 2.2, 2.5)

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
    "RTA_DP.c",
    "G_MP.c"
)

# 创建结果目录（如果不存在）
if (-Not (Test-Path $outputDir)) {
    New-Item -ItemType Directory -Path $outputDir | Out-Null
}

foreach ($util in $utilizations) {
    Write-Host "🔧 设置 TOTAL_UTILIZATION = $util"

    # 替换 main.h 中 TOTAL_UTILIZATION 的宏定义
    (Get-Content $mainHeaderPath) |
        ForEach-Object {
            $_ -replace "#define TOTAL_UTILIZATION\s+[0-9.]+", "#define TOTAL_UTILIZATION $util"
        } |
        Set-Content $mainHeaderPath

    # 编译所有源文件（你可以根据需要加上 -lm -Wall 等选项）
    gcc $sourceFiles -o $exeName

    # 如果编译成功就运行并保存输出
    if (Test-Path $exeName) {
        .\${exeName} > "$outputDir\output_$($util.ToString().Replace('.', '_')).txt"
    } 
}
