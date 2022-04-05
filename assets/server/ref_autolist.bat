<# :
@echo off
:: PowerShell config
set psConfig= ^
[console]::WindowWidth=60; ^
[console]::WindowHeight=9; ^
[console]::BufferWidth=[console]::WindowWidth; ^
[ScriptBlock]::Create((cat """%~f0""")

:: PowerShell start
powershell /nologo /noprofile /command ^ "&{%psConfig% -join [Char[]]10).Invoke(@(&{$args}%*))}"
exit /b
#>

# PowerShell settings
$psHost = Get-Host
$psWindow = $psHost.UI.RawUI
$psWindow.WindowTitle = "Referenced PK3 lists update"

# Update function
function ref_update ($type) {
	# Get folders
	$currentDir = Get-Location
	$parentDir = "$(Split-Path -Path $currentDir -Parent)\base"

	If ($type -eq "forcelist") {
		Write-Host "Updating Forcelist ..." -fo Cyan
		$refList = "ref_forcelist.txt"
		$refHeader = "; Autodownload Forcelist
; ------------------------
; If you want to use the forcelist, remove the [disabled] on the start
; and add all allowed pk3 files in the following format into it
; base/mymap.pk3
; mymod/plugin.pk3`n"

		# Clear file
		Clear-Content $refList
		Add-Content $refList $refHeader.replace("`n","`r`n");

		# Get latest plugin file
		$files = Get-ChildItem $currentDir -Filter jkmod_plugin_*.pk3 

		foreach ($file in $files) {
			If ($file -eq $files[-1]) {
				$refLine = "$($file.Directory.Name)/$($file.name)"
				[System.IO.File]::AppendAllText($refList, $refLine, [System.Text.Encoding]::UTF8)
			}
		}

		# Get selected from base
		Add-Type -AssemblyName System.Windows.Forms
		$FileBrowser = New-Object System.Windows.Forms.OpenFileDialog -Property @{
			Multiselect = $true
			Filter = "PK3 (*.pk3)|*.pk3"
			InitialDirectory = $parentDir
			Title = "Select PK3 files"
		}
		 
		[void]$FileBrowser.ShowDialog()

		$path = $FileBrowser.FileNames;

		If ($FileBrowser.FileNames -like "*\*" ) {
			# Get selected from base
			$files = Get-ChildItem $path -recurse -exclude assets*.pk3
			
			If ($files) {
				Add-Content $refList ""
			}
			
			foreach($file in $files) {
				Get-ChildItem ($file) | ForEach-Object {
					$refLine = "$($file.Directory.Name)/$($file.name)"
					if ($file -eq $files[-1]) {
						[System.IO.File]::AppendAllText($refList, $refLine, [System.Text.Encoding]::UTF8)
					}
					else {
						Add-Content $refList $refLine
					}
				}
			}
		}

		# Open file
		Invoke-Item $refList
	}
	
	If ($type -eq "whitelist") {
		Write-Host "Updating Whitelist ..." -fo Cyan
		$refList = "ref_whitelist.txt"
		$refHeader = "; Autodownload Whitelist
; ------------------------
; If you want to use the whitelist, remove the [disabled] on the start
; and add all allowed pk3 files in the following format into it
; base/mymap.pk3
; mymod/plugin.pk3`n"

		# Clear file
		Clear-Content $refList
		Add-Content $refList $refHeader.replace("`n","`r`n");

		# Get latest plugin file
		$files = Get-ChildItem $currentDir -Filter jkmod_plugin_*.pk3 

		foreach ($file in $files) {
			If ($file -eq $files[-1]) {
				$refLine = "$($file.Directory.Name)/$($file.name)"
				[System.IO.File]::AppendAllText($refList, $refLine, [System.Text.Encoding]::UTF8)
			}
		}

		# Get all from base
		$files = Get-ChildItem $parentDir -Filter *.pk3 -recurse -exclude assets*.pk3
		
		If ($files) {
			Add-Content $refList ""
		}

		foreach ($file in $files) {
			$refLine = "$($file.Directory.Name)/$($file.name)"
			If ($file -eq $files[-1]) {
				[System.IO.File]::AppendAllText($refList, $refLine, [System.Text.Encoding]::UTF8)
			}
			else {
				Add-Content $refList $refLine
			}
		}
		
		# Open file
		Invoke-Item $refList
	}
}

# Show options
do {
	Clear-Host
	Write-Host " ___________________ Available Options ___________________"
	Write-Host "|                                                         |"
	Write-Host "| f) to update ref_forcelist.txt                          |"
	Write-Host "| w) to update ref_whitelist.txt                          |"
	Write-Host "| a) to update all lists                                  |"
	Write-Host "| q) to quit                                              |"
	Write-Host "|_________________________________________________________|`n"
	
	$choice = Read-Host "Enter the option"
	
} until (($choice -eq "f") -or ($choice -eq "w") -or ($choice -eq "a") -or ($choice -eq "q") )

# Execute options
switch ($choice) {
	"f" {
		Clear-Host
		ref_update "forcelist"
	}
	"w" {
		Clear-Host
		ref_update "whitelist"
	}
	"a" {
		Clear-Host
		ref_update "forcelist"
		ref_update "whitelist"
	}
	"q" {
		Return
	}
}