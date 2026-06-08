# Info tool testing script
#
# Version: 20260608

$ExitSuccess = 0
$ExitFailure = 1
$ExitIgnore = 77

$Profiles = @("smdevinfo")
$OptionsPerProfile = @("")
$OptionSets = ""

$InputGlob = "*"

$VSDirectories = @(
	"msvscpp",
	"vs2008",
	"vs2010",
	"vs2012",
	"vs2013",
	"vs2015",
	"vs2017",
	"vs2019",
	"vs2022",
	"vs2026"
)

$VSConfigurations = @(
	"Release",
	"VSDebug"
)

$VSPlatforms = @(
	"Win32",
	"x64"
)

Function GetTestExecutablesDirectory
{
	$TestExecutablesDirectory = ""

	ForEach (${VSDirectory} in $VSDirectories)
	{
		ForEach (${VSConfiguration} in $VSConfigurations)
		{
			ForEach (${VSPlatform} in $VSPlatforms)
			{
				$TestExecutablesDirectory = "..\${VSDirectory}\${VSConfiguration}\${VSPlatform}"

				If (Test-Path ${TestExecutablesDirectory})
				{
					Return ${TestExecutablesDirectory}
				}
			}
			$TestExecutablesDirectory = "..\${VSDirectory}\${VSConfiguration}"

			If (Test-Path ${TestExecutablesDirectory})
			{
				Return ${TestExecutablesDirectory}
			}
		}
	}
	Return ${TestExecutablesDirectory}
}

$TestExecutablesDirectory = GetTestExecutablesDirectory

If (-Not (Test-Path ${TestExecutablesDirectory}))
{
	Write-Host "Missing test executables directory." -foreground Red

	Exit ${ExitFailure}
}

$TestExecutable = "${TestExecutablesDirectory}\smdevinfo.exe"

Invoke-Expression ${TestExecutable} "\\\\.\\PhysicalDrive0"

Exit ${LastExitCode}

