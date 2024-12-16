
function New-Shortcut {
    param (
        [string]$LinkPath,
        [string]$TargetPath
    )

    $shell = New-Object -ComObject WScript.Shell
    $shortcut = $shell.CreateShortcut($LinkPath)
    $shortcut.TargetPath = $TargetPath
    $shortcut.Save()
}

function Copy-ResourceDirectory {
    param (
        [string]$Path,
        [string]$Destination
    )
    $src_name = Split-Path $Path -Leaf

    $final_dir = "$Destination\$src_name"
    $parent_dir = Split-Path $Destination -Parent

    if(Test-Path $parent_dir) {
        # Remove the destination directory if it exists
        if (Test-Path $final_dir) {
            Remove-Item -Path $final_dir -Recurse -Force
        }
        Copy-Item -Path $Path -Destination $final_dir -Recurse
    }else{
        Write-Output "Parent directory does not exist: $parent_dir"
    }
    
}