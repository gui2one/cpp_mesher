. "$PSScriptRoot\utils.ps1"

Copy-ResourceDirectory -Path "$PSScriptRoot\mesher_resources" -Destination "$PSScriptRoot\build\Release"
Copy-ResourceDirectory -Path "$PSScriptRoot\mesher_resources" -Destination "$PSScriptRoot\build\Debug"