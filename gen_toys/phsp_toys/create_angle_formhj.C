void create_angle_formhj(const char * infile_path, const char * outfile_path)
{
  TFile * infile = TFile::Open(infile_path);
  TTree * intree = infile->Get<TTree>("tree");
 
  tree *t = new tree(intree);
  t->Loop(outfile_path);
  exit(0);
}
