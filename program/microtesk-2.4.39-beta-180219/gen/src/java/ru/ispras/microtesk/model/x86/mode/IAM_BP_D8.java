/*
 * Copyright (c) Institute for System Programming of the Russian Academy of Sciences
 * All Rights Reserved
 *
 * Institute for System Programming of the Russian Academy of Sciences (ISP RAS)
 * 25 Alexander Solzhenitsyn st., Moscow, 109004, Russia
 * http://www.ispras.ru
 *
 * The present file was automatically generated on the basis of formal specifications.
 * It is distributed under the same terms and conditions as the derived specifications.
 *
 * N.B. PLEASE DO NOT MODIFY THIS FILE.
 */

package ru.ispras.microtesk.model.x86.mode;

import java.util.Map;
import java.math.BigInteger;
import ru.ispras.microtesk.model.Execution;
import ru.ispras.microtesk.model.ProcessingElement;
import ru.ispras.microtesk.model.data.*;
import ru.ispras.microtesk.model.*;
import ru.ispras.microtesk.model.memory.Location;
import ru.ispras.microtesk.model.x86.PE;
import ru.ispras.microtesk.model.x86.TempVars;

import static ru.ispras.microtesk.model.x86.TypeDefs.*;

public final class IAM_BP_D8 extends IsaPrimitive {
  public static final String NAME = "IAM_BP_D8";

  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.MODE,
          "IAM_BP_D8",
          IAM_BP_D8.class,
          addR
          );
      addArgument("disp", SBYTE);
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final Immediate disp = (Immediate) getArgument("disp", args);
      return new IAM_BP_D8(disp);
    }
  }

  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final Immediate disp;

  public IAM_BP_D8(final Immediate disp) {
    this.disp = disp;

    addArgument("disp", disp);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    return String.format("%d[BP]", disp.access().load().bigIntegerValue());
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    return String.format("%s", disp.access().load().toString());
  }

  @Override
  public void init(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    vars__.mod.access().store(Data.valueOf(Type.CARD(2), 0x1));
    vars__.rm.access().store(Data.valueOf(Type.CARD(3), 0x6));
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    vars__.instruction_image_size.access().store(Data.valueOf(SIZE, 0x1));
  }

  @Override
  public Location access(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    return annotate(new Location(Data.signExtend(addR, Data.cast(SWORD, Data.signExtend(WORD, disp.access().load()).add(pe__.BP.access().load())))), vars__);
  }
}