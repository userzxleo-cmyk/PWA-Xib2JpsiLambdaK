void reOrder(TString input_path, TString output_path)
{
    TChain chain("DTree");
    chain.AddFile(input_path);
    TFile outFile(output_path, "RECREATE");
    TTree * tree1 = dynamic_cast<TTree*>(chain.CopyTree("(mjpsip>4.2&&mjpsip<4.43) || (mjpsip>4.47&&mjpsip<4.6)"));
    TTree * tree2 = dynamic_cast<TTree*>(chain.CopyTree("(mjpsip>4.43 && mjpsip<4.47)"));
    TTree * tree3 = dynamic_cast<TTree*>(chain.CopyTree("mjpsip<4.2 || mjpsip>4.6"));
    tree1->CopyEntries(tree2);
    tree1->CopyEntries(tree3);
    tree1->Write();
    outFile.Close();
}
