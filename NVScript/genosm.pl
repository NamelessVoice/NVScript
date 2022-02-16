#!/usr/bin/perl -w

use strict;

my $OSMName;
my @Scripts;
my %ScrCache;
my %History;

my $CC = 'g++-3';
my $CFlags = '-x c++ -P -w -mno-cygwin';
my $CDefines = '-DGENOSM';

sub process_file
{
	my ($file,$fh,$ifile,$script);
	$file = shift or return;
	return if ($History{$file});
	$History{$file} = 1;
	#open($fh,'<',$file) or die "blegh";
	print STDERR "$CC $CFlags $CDefines -E $file \n";
	open($fh, "$CC $CFlags $CDefines -E $file |") or die "blegh";
	my $skip = 0;
	while (<$fh>) {
		my ($osm,$class,$name,$base);
		if ($skip) {
			if (s|^.*\*/||) {
				$skip = 0;
			}
			else {
				next;
			}
			# This is way too simplistic. Do write simple comments.
		}
		s|//.*$||;
		s|/\*.*\*/||;
		if (s|/\*.*$||) {
			$skip = 1;
			next;
		}
		if (($osm) = m|^\s*DECLARE_MODULE\(\s*(\w+)\s*\)|) {
			$OSMName = $osm if not $OSMName;
		}
		elsif (($class,$base) = m|^\s*DECLARE_SCRIPT\(\s*(\w+)\s*,\s*(\w+)\s*\)|) {
			if (not $ScrCache{$class}) {
				$script = { 'ClassName' => $class };
				$script->{'ClassBase'} = $base;
			}
		}
		elsif (($name,undef,$base) = m|^\s*DECLARE_FACTORY\(\s*(\w+)\s*,\s*((\w+)\s*)?\)|) {
			if ($script) {
				$script->{'ScriptName'} = $name;
				$script->{'BaseName'} = $base || "";
				#$Scripts{$script->{'ClassName'}} = $script;
				push(@Scripts,$script);
				$ScrCache{$script->{'ClassName'}} = 1;
				undef $script;
			}
		}
		elsif (($ifile) = m|^#include \"([-\.\w]+)\"|) {
			&process_file($ifile);
		}
	}
	close($fh);

	#@Scripts = sort {$a->{'ScriptName'} cmp $b->{'ScriptName'}} @Scripts;
	
}

sub output_header {
	my (@files,$includes,$ofile,$fh,$ofh);
	$ofile = shift or return;
	@files = @_;
	if ($ofile ne '--') {
		open($fh,">",$ofile) or die "damn";
		binmode($fh);
		$ofh = select $fh;
	}

	$includes = "";
	foreach my $f (@files) {
		$includes .= "#include \"$f\"\n";
	}

	print <<L_TOP;
/*** Generated Automatically by genosm.pl -- modifications will be overwritten ***/
#include "ScriptModule.h"
$includes
L_TOP
	print "sNewClassDesc cScriptModule::sm_ScriptsArray[",scalar(@Scripts),"];\n";
	print "unsigned int cScriptModule::sm_ScriptsArraySize = 0;\n\n";

	print "void cScriptModule::InitScriptDescs()\n{\n";
	for (my $n = 0; $n <= $#Scripts; $n++) {
		print "\t",$Scripts[$n]->{'ClassName'},"::InitScriptDesc(sm_ScriptsArray+",$n,");\n";
		print "\tsm_ScriptsArray[",$n,"].pszModule = m_pszName;\n"
	}
	print "\tsm_ScriptsArraySize = ",scalar(@Scripts),";\n}\n\n";

	foreach my $s (@Scripts) {
		print <<L_FCN;

IScript* $s->{'ClassName'}::Make$s->{'ScriptName'}(const char* pszName, int iHostObjId)
{
	$s->{'ClassName'}* pscrRet = new(std::nothrow) $s->{'ClassName'}(pszName, iHostObjId);
	if (pscrRet)
	{
		pscrRet->AddRef();
		pscrRet->InitScriptData();
	}
	return static_cast<IScript*>(pscrRet);
}
void $s->{'ClassName'}::InitScriptDesc(sNewClassDesc * pDesc)
{
	pDesc->pszModule = "$OSMName";
	pDesc->pszClass = "$s->{'ScriptName'}";
	pDesc->pszBaseClass = "$s->{'BaseName'}";
	pDesc->pfnFactory = cScriptModule::ScriptFactory;
	pDesc->pfnRealFactory = $s->{'ClassName'}::Make$s->{'ScriptName'};
}
L_FCN
	}
	if ($ofile ne '--') {
		select $ofh;
		close($fh);
	}
}

{
	my ($fin,$fout);
	my $stdout = 0;
	while ($_ = $ARGV[0], /^-/) {
		shift;
		last if /^--$/;
		if (/^-c/) { $stdout = 1; }
		if (/^-o/) { $fout = shift; }
		if (/^-D/ or /^-I/) { $CDefines .= " $_"; }
	}
	foreach my $f (@ARGV) {
		&process_file($f);
		$fin = $f;
	}
	if ($stdout) { $fout = '--'; }
	if (!defined $fout) { ($fout = $fin) =~ s|\.h$|_osm.cpp|; }
	&output_header($fout,@ARGV);
}
